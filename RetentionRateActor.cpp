// Unreal conversion of Unity RetentionRate using a minimal MLP

#include "RetentionRateActor.h"

// Simple 2-layer MLP with sigmoid activation and backpropagation
class FSimpleMLP_Retention
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

// Memory entry equivalent
struct FMemoryEntry
{
    FString Id;
    TArray<double> Features;
    float RetentionTime = 0.f;
    bool bIsRetained = false;
};

// Retention manager equivalent
class FRetentionManager
{
public:
    void InitializeNeuralNetwork(int32 InputSize, int32 HiddenNeurons, int32 OutputSize)
    {
        Network.Initialize(InputSize, HiddenNeurons, OutputSize, 0.1);
    }

    void AddMemoryEntry(const FString& Id, const TArray<double>& Features, float RetentionTime, bool bIsRetained)
    {
        FMemoryEntry E;
        E.Id = Id;
        E.Features = Features;
        E.RetentionTime = RetentionTime;
        E.bIsRetained = bIsRetained;
        Entries.Add(MoveTemp(E));
    }

    double CalculateRetentionRate() const
    {
        if (Entries.Num() == 0) return 0.0;
        int32 Retained = 0;
        for (const FMemoryEntry& E : Entries)
        {
            if (E.bIsRetained) ++Retained;
        }
        return static_cast<double>(Retained) / static_cast<double>(Entries.Num());
    }

    double CalculateAverageRetentionTime() const
    {
        if (Entries.Num() == 0) return 0.0;
        double Sum = 0.0;
        for (const FMemoryEntry& E : Entries)
        {
            Sum += static_cast<double>(E.RetentionTime);
        }
        return Sum / static_cast<double>(Entries.Num());
    }

    void TrainNetwork(int32 Epochs)
    {
        if (Entries.Num() == 0) return;

        TArray<TArray<double>> Inputs;
        TArray<TArray<double>> Targets;
        Inputs.Reserve(Entries.Num());
        Targets.Reserve(Entries.Num());
        for (const FMemoryEntry& E : Entries)
        {
            Inputs.Add(E.Features);
            TArray<double> T; T.SetNum(1); T[0] = E.bIsRetained ? 1.0 : 0.0; Targets.Add(MoveTemp(T));
        }

        for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
        {
            const double Error = Network.TrainEpoch(Inputs, Targets);
            if (Epoch % 100 == 0)
            {
                UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), Epoch, Error);
            }
        }
    }

    float PredictRetentionLikelihood(const TArray<double>& Features)
    {
        const TArray<double> Out = Network.Compute(Features);
        return Out.Num() > 0 ? static_cast<float>(Out[0]) : 0.0f;
    }

private:
    TArray<FMemoryEntry> Entries;
    FSimpleMLP_Retention Network;
};

ARetentionRateActor::ARetentionRateActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARetentionRateActor::BeginPlay()
{
    Super::BeginPlay();

    FRetentionManager Manager;
    Manager.InitializeNeuralNetwork(/*inputSize*/ 3, /*hiddenNeurons*/ 5, /*outputSize*/ 1);

    // Simulate Memory Entries
    Manager.AddMemoryEntry(TEXT("Entry1"), TArray<double>({1.0, 2.0, 3.0}), 12.5f, true);
    Manager.AddMemoryEntry(TEXT("Entry2"), TArray<double>({2.0, 3.0, 4.0}), 8.0f, false);
    Manager.AddMemoryEntry(TEXT("Entry3"), TArray<double>({4.0, 5.0, 6.0}), 15.0f, true);
    Manager.AddMemoryEntry(TEXT("Entry4"), TArray<double>({5.0, 6.0, 7.0}), 10.0f, false);

    // Calculate Retention Metrics
    const double RetentionRate = Manager.CalculateRetentionRate();
    UE_LOG(LogTemp, Log, TEXT("Initial Retention Rate: %f%%"), RetentionRate * 100.0);

    const double AverageRetentionTime = Manager.CalculateAverageRetentionTime();
    UE_LOG(LogTemp, Log, TEXT("Average Retention Time: %f seconds"), AverageRetentionTime);

    // Train Neural Network
    Manager.TrainNetwork(/*epochs*/ 1000);

    // Test Retention Prediction
    const TArray<double> TestFeatures = TArray<double>({2.0, 3.0, 4.0});
    const float Predicted = Manager.PredictRetentionLikelihood(TestFeatures);
    UE_LOG(LogTemp, Log, TEXT("Predicted Retention Likelihood for Features [2.0, 3.0, 4.0]: %f%%"), Predicted * 100.0f);
}

