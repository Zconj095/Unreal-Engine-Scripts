// Unreal conversion of Unity SpatialMemory: quadtree + simple MLP

#include "SpatialMemoryActor.h"
#include "DrawDebugHelpers.h"

// -------------------------
// Minimal 2-layer MLP (sigmoid)
// -------------------------
class FSimpleMLP_Spatial
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

        // Random init
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
// Quadtree for spatial memory
// -------------------------
struct FQuadtreeNode
{
    FVector2D Center;
    double Size = 0.0; // half-extent
    TArray<FVector2D> Points;
    TMap<FVector2D, FString> Memory; // store simple string memory for demo
    TArray<TUniquePtr<FQuadtreeNode>> Children; // 0 or 4
    static constexpr int32 MaxCapacity = 4;

    FQuadtreeNode(const FVector2D& InCenter, double InSize)
        : Center(InCenter), Size(InSize)
    {}

    bool Contains(const FVector2D& P) const
    {
        return (P.X >= Center.X - Size && P.X <= Center.X + Size &&
                P.Y >= Center.Y - Size && P.Y <= Center.Y + Size);
    }

    void Subdivide()
    {
        const double NewSize = Size / 2.0;
        Children.SetNum(4);
        Children[0] = MakeUnique<FQuadtreeNode>(FVector2D(Center.X - NewSize, Center.Y - NewSize), NewSize);
        Children[1] = MakeUnique<FQuadtreeNode>(FVector2D(Center.X + NewSize, Center.Y - NewSize), NewSize);
        Children[2] = MakeUnique<FQuadtreeNode>(FVector2D(Center.X - NewSize, Center.Y + NewSize), NewSize);
        Children[3] = MakeUnique<FQuadtreeNode>(FVector2D(Center.X + NewSize, Center.Y + NewSize), NewSize);
    }

    void DistributePoint(const FVector2D& P, const FString& Mem)
    {
        for (auto& Child : Children)
        {
            if (Child && Child->Contains(P))
            {
                Child->InsertWithMemory(P, Mem);
                break;
            }
        }
    }

    void InsertWithMemory(const FVector2D& P, const FString& Mem)
    {
        if (!Contains(P)) return;

        if (Children.Num() == 0)
        {
            Points.Add(P);
            Memory.Add(P, Mem);
            if (Points.Num() > MaxCapacity)
            {
                Subdivide();
                // Redistribute existing points to children, then clear local storage
                for (const FVector2D& OldP : Points)
                {
                    const FString* OldMem = Memory.Find(OldP);
                    if (OldMem)
                    {
                        DistributePoint(OldP, *OldMem);
                    }
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
// Actor implementation
// -------------------------

ASpatialMemoryActor::ASpatialMemoryActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

namespace
{
    // Keep demo state local to this translation unit
    TUniquePtr<FQuadtreeNode> GRootNode;
    FSimpleMLP_Spatial GNetwork;
}

void ASpatialMemoryActor::BeginPlay()
{
    Super::BeginPlay();

    // Initialize Spatial Memory (quadtree)
    GRootNode = MakeUnique<FQuadtreeNode>(FVector2D(0.0, 0.0), /*half-extent*/ 10.0);
    GRootNode->InsertWithMemory(FVector2D(2.0, 3.0), TEXT("Memory for Point (2,3)"));
    GRootNode->InsertWithMemory(FVector2D(-4.0, -2.0), TEXT("Memory for Point (-4,-2)"));
    GRootNode->InsertWithMemory(FVector2D(5.0, 7.0), TEXT("Memory for Point (5,7)"));

    // Setup and train neural network (2-5-1)
    GNetwork.Initialize(2, 5, 1, 0.1);

    TArray<TArray<double>> Inputs;
    TArray<TArray<double>> Outputs;
    Inputs.Add(TArray<double>({2.0, 3.0}));   Outputs.Add(TArray<double>({1.0}));
    Inputs.Add(TArray<double>({-4.0, -2.0})); Outputs.Add(TArray<double>({0.5}));
    Inputs.Add(TArray<double>({5.0, 7.0}));  Outputs.Add(TArray<double>({0.8}));

    for (int32 Epoch = 0; Epoch < 1000; ++Epoch)
    {
        const double Error = GNetwork.TrainEpoch(Inputs, Outputs);
        if (Epoch % 100 == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), Epoch, Error);
        }
    }

    // Test network
    const TArray<double> TestIn = TArray<double>({2.0, 3.0});
    const TArray<double> Out = GNetwork.Compute(TestIn);
    if (Out.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Neural Network Output: %f"), Out[0]);
    }

    // Example retrieval
    if (const FString* Found = GRootNode->RetrieveMemory(FVector2D(2.0, 3.0)))
    {
        UE_LOG(LogTemp, Log, TEXT("Retrieved Memory: %s"), **Found);
    }
}

static void DrawQuadtreeRecursive(UWorld* World, const FQuadtreeNode* Node)
{
    if (!World || !Node) return;
    if (Node->Children.Num() == 0) return;

    const FVector ParentLoc(Node->Center.X, Node->Center.Y, 0.0f);
    for (const auto& Child : Node->Children)
    {
        if (!Child) continue;
        const FVector ChildLoc(Child->Center.X, Child->Center.Y, 0.0f);
        DrawDebugLine(World, ParentLoc, ChildLoc, FColor::Blue, /*bPersistentLines*/ false, /*LifeTime*/ 0.0f, /*DepthPriority*/ 0, /*Thickness*/ 2.0f);
        DrawQuadtreeRecursive(World, Child.Get());
    }
}

void ASpatialMemoryActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    DrawQuadtreeRecursive(GetWorld(), GRootNode.Get());
}

