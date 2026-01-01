// Minimal recall rate demo converted from Unity (Accord) to Unreal C++

#include "RecallRateActor.h"
#include "Containers/Array.h"
#include "Misc/OutputDeviceNull.h"

// Simple 2-layer MLP with sigmoid activation and backpropagation
class FSimpleMLP
{
public:
    void Initialize(int32 InInputSize, int32 InHiddenNeurons, int32 InOutputSize, double InLearningRate = 0.1)
    {
        InputSize = InInputSize;
        HiddenSize = InHiddenNeurons;
        OutputSize = InOutputSize;
        LearningRate = InLearningRate;

        // Allocate
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

    // Forward compute: returns outputs for given inputs
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

    // One epoch over all samples (stochastic updates per sample). Returns SSE for monitoring.
    double TrainEpoch(const TArray<TArray<double>>& Inputs, const TArray<TArray<double>>& Targets)
    {
        check(Inputs.Num() == Targets.Num());
        double sse = 0.0;

        for (int32 n = 0; n < Inputs.Num(); ++n)
        {
            const TArray<double>& x = Inputs[n];
            const TArray<double>& t = Targets[n];

            // Forward
            const TArray<double> y = Compute(x);

            // Accumulate error
            for (int32 o = 0; o < OutputSize; ++o)
            {
                const double e = t[o] - y[o];
                sse += 0.5 * e * e;
            }

            // Output deltas
            DeltaOut.SetNum(OutputSize);
            for (int32 o = 0; o < OutputSize; ++o)
            {
                const double e = t[o] - y[o];
                DeltaOut[o] = e * y[o] * (1.0 - y[o]);
            }

            // Hidden deltas
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

            // Update W2, B2
            for (int32 o = 0; o < OutputSize; ++o)
            {
                for (int32 h = 0; h < HiddenSize; ++h)
                {
                    W2[o][h] += LearningRate * DeltaOut[o] * Hidden[h];
                }
                B2[o] += LearningRate * DeltaOut[o];
            }

            // Update W1, B1
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
    static double Sigmoid(double x)
    {
        return 1.0 / (1.0 + FMath::Exp(-x));
    }

private:
    int32 InputSize = 0;
    int32 HiddenSize = 0;
    int32 OutputSize = 0;
    double LearningRate = 0.1;

    // Parameters
    TArray<TArray<double>> W1; // [Hidden][Input]
    TArray<double> B1;         // [Hidden]
    TArray<TArray<double>> W2; // [Output][Hidden]
    TArray<double> B2;         // [Output]

    // Work buffers
    TArray<double> Hidden;     // [Hidden]
    TArray<double> Output;     // [Output]
    TArray<double> DeltaOut;   // [Output]
    TArray<double> DeltaHidden;// [Hidden]
};

// Data entry analogous to Unity's RecallEntry
struct FRecallEntry
{
    TArray<double> InputData;
    bool bIsRecalled = false;
};

// Manager analogous to Unity's RecallManager
class FRecallManager
{
public:
    void InitializeNeuralNetwork(int32 InputSize, int32 HiddenNeurons, int32 OutputSize)
    {
        Network.Initialize(InputSize, HiddenNeurons, OutputSize, /*LearningRate*/ 0.1);
    }

    void AddRecallData(const TArray<double>& InputData, bool bIsRecalled)
    {
        FRecallEntry Entry;
        Entry.InputData = InputData;
        Entry.bIsRecalled = bIsRecalled;
        Entries.Add(MoveTemp(Entry));
    }

    double CalculateRecallRate() const
    {
        if (Entries.Num() == 0) return 0.0;
        int32 RecalledCount = 0;
        for (const FRecallEntry& E : Entries)
        {
            if (E.bIsRecalled) ++RecalledCount;
        }
        return static_cast<double>(RecalledCount) / static_cast<double>(Entries.Num());
    }

    void TrainNetwork(int32 Epochs)
    {
        if (Entries.Num() == 0) return;

        // Build input/output arrays
        TArray<TArray<double>> Inputs;
        TArray<TArray<double>> Outputs;
        Inputs.Reserve(Entries.Num());
        Outputs.Reserve(Entries.Num());

        for (const FRecallEntry& E : Entries)
        {
            Inputs.Add(E.InputData);
            TArray<double> Out;
            Out.SetNum(1);
            Out[0] = E.bIsRecalled ? 1.0 : 0.0;
            Outputs.Add(MoveTemp(Out));
        }

        for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
        {
            const double Error = Network.TrainEpoch(Inputs, Outputs);
            if (Epoch % 100 == 0)
            {
                UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), Epoch, Error);
            }
        }
    }

    double PredictRecall(const TArray<double>& InputData)
    {
        const TArray<double> Out = Network.Compute(InputData);
        return Out.Num() > 0 ? Out[0] : 0.0;
    }

private:
    TArray<FRecallEntry> Entries;
    FSimpleMLP Network;
};


ARecallRateActor::ARecallRateActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ARecallRateActor::BeginPlay()
{
    Super::BeginPlay();

    FRecallManager RecallManager;
    RecallManager.InitializeNeuralNetwork(/*inputSize*/ 2, /*hiddenNeurons*/ 5, /*outputSize*/ 1);

    // Example data
    RecallManager.AddRecallData(TArray<double>({1.0, 2.0}), true);
    RecallManager.AddRecallData(TArray<double>({2.0, 3.0}), true);
    RecallManager.AddRecallData(TArray<double>({4.0, 5.0}), false);
    RecallManager.AddRecallData(TArray<double>({6.0, 7.0}), true);
    RecallManager.AddRecallData(TArray<double>({8.0, 9.0}), false);

    const double InitialRecallRate = RecallManager.CalculateRecallRate();
    UE_LOG(LogTemp, Log, TEXT("Initial Recall Rate: %f%%"), InitialRecallRate * 100.0);

    // Train
    RecallManager.TrainNetwork(/*epochs*/ 1000);

    // Test prediction
    const TArray<double> TestInput = TArray<double>({2.0, 3.0});
    const double RecallConfidence = RecallManager.PredictRecall(TestInput);
    UE_LOG(LogTemp, Log, TEXT("Recall Confidence for Input [2.0, 3.0]: %f%%"), RecallConfidence * 100.0);

    // Updated recall rate (using 0.5 threshold like Unity example)
    RecallManager.AddRecallData(TestInput, RecallConfidence >= 0.5);
    const double UpdatedRecallRate = RecallManager.CalculateRecallRate();
    UE_LOG(LogTemp, Log, TEXT("Updated Recall Rate: %f%%"), UpdatedRecallRate * 100.0);
}

