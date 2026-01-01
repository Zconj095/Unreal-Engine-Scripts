// Unreal conversion of Unity SpatialRecursion: quadtree + simple MLP + TF stub

#include "SpatialRecursionActor.h"
#include "DrawDebugHelpers.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

// -------------------------
// Minimal 2-layer MLP (sigmoid + backprop)
// -------------------------
class FSimpleMLP_Recursion
{
public:
    void Initialize(int32 InInputSize, int32 InHiddenNeurons, int32 InOutputSize, double InLearningRate = 0.1)
    {
        InputSize = InInputSize;
        HiddenSize = InHiddenNeurons;
        OutputSize = InOutputSize;
        LearningRate = InLearningRate;

        W1.SetNum(HiddenSize);
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            W1[h].SetNum(InputSize);
        }
        B1.Init(0.0, HiddenSize);

        W2.SetNum(OutputSize);
        for (int32 o = 0; o < OutputSize; ++o)
        {
            W2[o].SetNum(HiddenSize);
        }
        B2.Init(0.0, OutputSize);

        for (int32 h = 0; h < HiddenSize; ++h)
        {
            for (int32 i = 0; i < InputSize; ++i)
            {
                W1[h][i] = FMath::FRandRange(-0.5, 0.5);
            }
            B1[h] = FMath::FRandRange(-0.5, 0.5);
        }
        for (int32 o = 0; o < OutputSize; ++o)
        {
            for (int32 h = 0; h < HiddenSize; ++h)
            {
                W2[o][h] = FMath::FRandRange(-0.5, 0.5);
            }
            B2[o] = FMath::FRandRange(-0.5, 0.5);
        }
    }

    TArray<double> Compute(const TArray<double>& Input)
    {
        check(Input.Num() == InputSize);

        Hidden.SetNum(HiddenSize);
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            double sum = B1[h];
            for (int32 i = 0; i < InputSize; ++i)
            {
                sum += W1[h][i] * Input[i];
            }
            Hidden[h] = Sigmoid(sum);
        }

        Output.SetNum(OutputSize);
        for (int32 o = 0; o < OutputSize; ++o)
        {
            double sum = B2[o];
            for (int32 h = 0; h < HiddenSize; ++h)
            {
                sum += W2[o][h] * Hidden[h];
            }
            Output[o] = Sigmoid(sum);
        }

        return Output;
    }

    double TrainEpoch(const TArray<TArray<double>>& Inputs, const TArray<TArray<double>>& Targets)
    {
        check(Inputs.Num() == Targets.Num());
        double sse = 0.0;

        for (int32 n = 0; n < Inputs.Num(); ++n)
        {
            const TArray<double>& x = Inputs[n];
            const TArray<double>& t = Targets[n];

            const TArray<double> y = Compute(x);

            for (int32 o = 0; o < OutputSize; ++o)
            {
                const double e = t[o] - y[o];
                sse += 0.5 * e * e;
            }

            DeltaOut.SetNum(OutputSize);
            for (int32 o = 0; o < OutputSize; ++o)
            {
                const double e = t[o] - y[o];
                DeltaOut[o] = e * y[o] * (1.0 - y[o]);
            }

            DeltaHidden.SetNum(HiddenSize);
            for (int32 h = 0; h < HiddenSize; ++h)
            {
                double sum = 0.0;
                for (int32 o = 0; o < OutputSize; ++o)
                {
                    sum += DeltaOut[o] * W2[o][h];
                }
                DeltaHidden[h] = sum * Hidden[h] * (1.0 - Hidden[h]);
            }

            for (int32 o = 0; o < OutputSize; ++o)
            {
                for (int32 h = 0; h < HiddenSize; ++h)
                {
                    W2[o][h] += LearningRate * DeltaOut[o] * Hidden[h];
                }
                B2[o] += LearningRate * DeltaOut[o];
            }

            for (int32 h = 0; h < HiddenSize; ++h)
            {
                for (int32 i = 0; i < InputSize; ++i)
                {
                    W1[h][i] += LearningRate * DeltaHidden[h] * x[i];
                }
                B1[h] += LearningRate * DeltaHidden[h];
            }
        }

        return sse;
    }

private:
    static double Sigmoid(double x) { return 1.0 / (1.0 + FMath::Exp(-x)); }

private:
    int32 InputSize = 0;
    int32 HiddenSize = 0;
    int32 OutputSize = 0;
    double LearningRate = 0.1;

    TArray<TArray<double>> W1; // [Hidden][Input]
    TArray<double> B1;         // [Hidden]
    TArray<TArray<double>> W2; // [Output][Hidden]
    TArray<double> B2;         // [Output]

    TArray<double> Hidden;
    TArray<double> Output;
    TArray<double> DeltaOut;
    TArray<double> DeltaHidden;
};

// -------------------------
// Quadtree for 2D points with optional memory strings
// -------------------------
struct FQuadtreeNode2D
{
    FVector2D Center;
    double Size = 0.0; // half-extent
    TArray<FVector2D> Points;
    TMap<FVector2D, FString> Memory; // optional string memory per point
    TArray<TUniquePtr<FQuadtreeNode2D>> Children; // 0 or 4
    static constexpr int32 MaxCapacity = 4;

    FQuadtreeNode2D(const FVector2D& InCenter, double InSize)
        : Center(InCenter), Size(InSize) {}

    bool Contains(const FVector2D& P) const
    {
        return (P.X >= Center.X - Size && P.X <= Center.X + Size &&
                P.Y >= Center.Y - Size && P.Y <= Center.Y + Size);
    }

    void Subdivide()
    {
        const double NewSize = Size / 2.0;
        Children.SetNum(4);
        Children[0] = MakeUnique<FQuadtreeNode2D>(FVector2D(Center.X - NewSize, Center.Y - NewSize), NewSize);
        Children[1] = MakeUnique<FQuadtreeNode2D>(FVector2D(Center.X + NewSize, Center.Y - NewSize), NewSize);
        Children[2] = MakeUnique<FQuadtreeNode2D>(FVector2D(Center.X - NewSize, Center.Y + NewSize), NewSize);
        Children[3] = MakeUnique<FQuadtreeNode2D>(FVector2D(Center.X + NewSize, Center.Y + NewSize), NewSize);
    }

    void DistributePoint(const FVector2D& P, const FString* Mem)
    {
        for (auto& Child : Children)
        {
            if (Child && Child->Contains(P))
            {
                Child->Insert(P, Mem);
                break;
            }
        }
    }

    void Insert(const FVector2D& P, const FString* Mem)
    {
        if (!Contains(P)) return;

        if (Children.Num() == 0)
        {
            Points.Add(P);
            if (Mem)
            {
                Memory.Add(P, *Mem);
            }

            if (Points.Num() > MaxCapacity)
            {
                Subdivide();
                for (const FVector2D& OldP : Points)
                {
                    const FString* OldMem = Memory.Find(OldP);
                    DistributePoint(OldP, OldMem);
                }
                Points.Reset();
                Memory.Reset();
            }
        }
        else
        {
            DistributePoint(P, Mem);
        }
    }

    const FString* RetrieveMemory(const FVector2D& P) const
    {
        if (!Contains(P)) return nullptr;
        if (Children.Num() == 0)
        {
            return Memory.Find(P);
        }
        for (const auto& Child : Children)
        {
            if (Child && Child->Contains(P))
            {
                return Child->RetrieveMemory(P);
            }
        }
        return nullptr;
    }
};

// -------------------------
// TensorFlow-like stub (no runtime dependency)
// -------------------------
class FRecursiveModel
{
public:
    explicit FRecursiveModel(const FString& InModelPath)
        : ModelPath(InModelPath)
    {
        const bool bExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*ModelPath);
        if (bExists)
        {
            UE_LOG(LogTemp, Log, TEXT("Loaded model bytes from: %s (stub)"), *ModelPath);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Model not found at: %s (stub continues anyway)"), *ModelPath);
        }
    }

    // Returns a trivial transform of inputs as a placeholder
    TArray<float> Predict(const TArray<float>& InputData) const
    {
        TArray<float> Out; Out.Reserve(InputData.Num());
        // Stub behavior: scale and reverse order to demonstrate flow
        for (int32 i = InputData.Num() - 1; i >= 0; --i)
        {
            Out.Add(InputData[i] * 0.5f);
        }
        return Out;
    }

private:
    FString ModelPath;
};

// -------------------------
// Actor implementation
// -------------------------

ASpatialRecursionActor::ASpatialRecursionActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

namespace
{
    TUniquePtr<FQuadtreeNode2D> GRoot;
    FSimpleMLP_Recursion GNet;
    TUniquePtr<FRecursiveModel> GTfModel;
}

void ASpatialRecursionActor::BeginPlay()
{
    Super::BeginPlay();

    // Initialize Quadtree
    GRoot = MakeUnique<FQuadtreeNode2D>(FVector2D(0.0, 0.0), /*half-extent*/ 10.0);
    GRoot->Insert(FVector2D(2.0, 3.0), new FString(TEXT("Memory for (2, 3)")));
    GRoot->Insert(FVector2D(-4.0, -2.0), new FString(TEXT("Memory for (-4, -2)")));
    GRoot->Insert(FVector2D(5.0, 7.0), new FString(TEXT("Memory for (5, 7)")));

    // Initialize TensorFlow-like stub model (looks under Content/Model/recursive_model.pb)
    const FString ModelPath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Model/recursive_model.pb"));
    GTfModel = MakeUnique<FRecursiveModel>(ModelPath);

    // Initialize neural network
    GNet.Initialize(2, 5, 1, 0.1);

    // Train
    TArray<TArray<double>> Inputs;
    TArray<TArray<double>> Outputs;
    Inputs.Add(TArray<double>({2.0, 3.0}));    Outputs.Add(TArray<double>({1.0}));
    Inputs.Add(TArray<double>({-4.0, -2.0}));  Outputs.Add(TArray<double>({0.5}));
    Inputs.Add(TArray<double>({5.0, 7.0}));    Outputs.Add(TArray<double>({0.8}));
    for (int32 Epoch = 0; Epoch < 1000; ++Epoch)
    {
        const double Error = GNet.TrainEpoch(Inputs, Outputs);
        if (Epoch % 100 == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), Epoch, Error);
        }
    }

    // Test neural network
    const TArray<double> TestIn = TArray<double>({2.0, 3.0});
    const TArray<double> NetOut = GNet.Compute(TestIn);
    if (NetOut.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Neural Network Prediction: %f"), NetOut[0]);
    }

    // Test TF stub
    const TArray<float> TfIn = TArray<float>({1.0f, 2.0f, 3.0f});
    const TArray<float> TfOut = GTfModel->Predict(TfIn);
    FString TfOutStr;
    for (int32 i = 0; i < TfOut.Num(); ++i)
    {
        TfOutStr += FString::SanitizeFloat(TfOut[i]);
        if (i + 1 < TfOut.Num()) TfOutStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("TensorFlow (stub) Prediction: %s"), *TfOutStr);

    // Example memory retrieval
    if (const FString* Found = GRoot->RetrieveMemory(FVector2D(2.0, 3.0)))
    {
        UE_LOG(LogTemp, Log, TEXT("Retrieved Memory: %s"), **Found);
    }
}

static void DrawQuadtreeRecursive(UWorld* World, const FQuadtreeNode2D* Node)
{
    if (!World || !Node) return;
    if (Node->Children.Num() == 0) return;

    const FVector ParentLoc(Node->Center.X, Node->Center.Y, 0.0f);
    for (const auto& Child : Node->Children)
    {
        if (!Child) continue;
        const FVector ChildLoc(Child->Center.X, Child->Center.Y, 0.0f);
        DrawDebugLine(World, ParentLoc, ChildLoc, FColor::Green, false, 0.0f, 0, 1.5f);
        DrawQuadtreeRecursive(World, Child.Get());
    }
}

void ASpatialRecursionActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    DrawQuadtreeRecursive(GetWorld(), GRoot.Get());
}

