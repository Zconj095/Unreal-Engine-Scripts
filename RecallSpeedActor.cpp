// Unreal conversion of Unity RecallSpeed using a minimal MLP

#include "RecallSpeedActor.h"
#include "HAL/PlatformProcess.h"

// Simple 2-layer MLP with sigmoid activation and backpropagation
class FSimpleMLP_RecallSpeed
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

// Recall entry for speed tracking
struct FRecallEntry_Speed
{
    TArray<double> InputData;
    float RecallTime = 0.f; // seconds
};

// Manager equivalent to Unity's RecallSpeedManager
class FRecallSpeedManager
{
public:
    void InitializeNeuralNetwork(int32 InputSize, int32 HiddenNeurons, int32 OutputSize)
    {
        Network.Initialize(InputSize, HiddenNeurons, OutputSize, 0.1);
    }

    void AddRecallData(const TArray<double>& InputData, float RecallTime)
    {
        FRecallEntry_Speed E; E.InputData = InputData; E.RecallTime = RecallTime; Entries.Add(MoveTemp(E));
    }

    double CalculateAverageRecallSpeed() const
    {
        if (Entries.Num() == 0) return 0.0;
        double Sum = 0.0;
        for (const auto& E : Entries) { Sum += static_cast<double>(E.RecallTime); }
        return Sum / static_cast<double>(Entries.Num());
    }

    void TrainNetwork(int32 Epochs)
    {
        if (Entries.Num() == 0) return;
        TArray<TArray<double>> Inputs; Inputs.Reserve(Entries.Num());
        TArray<TArray<double>> Targets; Targets.Reserve(Entries.Num());
        for (const auto& E : Entries)
        {
            Inputs.Add(E.InputData);
            TArray<double> T; T.SetNum(1); T[0] = static_cast<double>(E.RecallTime); Targets.Add(MoveTemp(T));
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

    float PredictRecallSpeed(const TArray<double>& InputData)
    {
        const TArray<double> Out = Network.Compute(InputData);
        return Out.Num() > 0 ? static_cast<float>(Out[0]) : 0.0f;
    }

private:
    TArray<FRecallEntry_Speed> Entries;
    FSimpleMLP_RecallSpeed Network;
};

ARecallSpeedActor::ARecallSpeedActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

static void SimulateRecallOperation(const TArray<double>& /*InputData*/)
{
    // Simulate latency 50-200 ms; note: blocking the game thread briefly
    const float DelaySec = FMath::FRandRange(0.05f, 0.2f);
    FPlatformProcess::Sleep(DelaySec);
}

static void AddRecallDataWithTimer(FRecallSpeedManager& Manager, const TArray<double>& InputData)
{
    const double Start = FPlatformTime::Seconds();
    SimulateRecallOperation(InputData);
    const double End = FPlatformTime::Seconds();
    const float RecallTime = static_cast<float>(End - Start);
    Manager.AddRecallData(InputData, RecallTime);
}

void ARecallSpeedActor::BeginPlay()
{
    Super::BeginPlay();

    FRecallSpeedManager Manager;
    Manager.InitializeNeuralNetwork(/*inputSize*/ 2, /*hiddenNeurons*/ 5, /*outputSize*/ 1);

    // Simulate adding recall data (blocking sleeps kept short)
    AddRecallDataWithTimer(Manager, TArray<double>({1.0, 2.0}));
    AddRecallDataWithTimer(Manager, TArray<double>({2.0, 3.0}));
    AddRecallDataWithTimer(Manager, TArray<double>({4.0, 5.0}));

    const double AvgSpeed = Manager.CalculateAverageRecallSpeed();
    UE_LOG(LogTemp, Log, TEXT("Average Recall Speed: %f seconds"), AvgSpeed);

    Manager.TrainNetwork(/*epochs*/ 1000);

    const TArray<double> TestInput = TArray<double>({2.0, 3.0});
    const float Predicted = Manager.PredictRecallSpeed(TestInput);
    UE_LOG(LogTemp, Log, TEXT("Predicted Recall Speed for Input [2.0, 3.0]: %f seconds"), Predicted);
}

