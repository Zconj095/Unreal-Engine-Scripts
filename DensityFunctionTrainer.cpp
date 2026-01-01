#include "DensityFunctionTrainer.h"

DEFINE_LOG_CATEGORY(LogDensityFunctionTrainer);

UDensityFunctionTrainer::UDensityFunctionTrainer()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDensityFunctionTrainer::BeginPlay()
{
    Super::BeginPlay();

    InitializeNetwork();
    RunExampleTraining();
}

void UDensityFunctionTrainer::InitializeNetwork()
{
    InputHiddenWeights.SetNumZeroed(InputSize * HiddenSize);
    HiddenOutputWeights.SetNumZeroed(HiddenSize * OutputSize);
    HiddenBiases.SetNumZeroed(HiddenSize);
    OutputBiases.SetNumZeroed(OutputSize);

    for (float& Weight : InputHiddenWeights)
    {
        Weight = FMath::FRandRange(-1.f, 1.f);
    }

    for (float& Weight : HiddenOutputWeights)
    {
        Weight = FMath::FRandRange(-1.f, 1.f);
    }

    for (float& Bias : HiddenBiases)
    {
        Bias = FMath::FRandRange(-0.5f, 0.5f);
    }

    for (float& Bias : OutputBiases)
    {
        Bias = FMath::FRandRange(-0.5f, 0.5f);
    }
}

float UDensityFunctionTrainer::Sigmoid(float Value) const
{
    return 1.f / (1.f + FMath::Exp(-Value));
}

float UDensityFunctionTrainer::SigmoidDerivative(float Value) const
{
    return Value * (1.f - Value);
}

void UDensityFunctionTrainer::ForwardPass(const TArray<float>& Input, TArray<float>& HiddenOutputs, TArray<float>& OutputValues) const
{
    HiddenOutputs.Init(0.f, HiddenSize);
    OutputValues.Init(0.f, OutputSize);

    for (int32 HiddenIdx = 0; HiddenIdx < HiddenSize; ++HiddenIdx)
    {
        float Sum = HiddenBiases[HiddenIdx];
        for (int32 InputIdx = 0; InputIdx < InputSize; ++InputIdx)
        {
            Sum += InputHiddenWeights[InputIdx * HiddenSize + HiddenIdx] * Input[InputIdx];
        }
        HiddenOutputs[HiddenIdx] = Sigmoid(Sum);
    }

    for (int32 OutputIdx = 0; OutputIdx < OutputSize; ++OutputIdx)
    {
        float Sum = OutputBiases[OutputIdx];
        for (int32 HiddenIdx = 0; HiddenIdx < HiddenSize; ++HiddenIdx)
        {
            Sum += HiddenOutputWeights[HiddenIdx * OutputSize + OutputIdx] * HiddenOutputs[HiddenIdx];
        }
        OutputValues[OutputIdx] = Sigmoid(Sum);
    }
}

void UDensityFunctionTrainer::TrainNetwork(const TArray<float>& InputFlat, const TArray<float>& TargetFlat, int32 SampleCount, int32 Epochs, float LearningRate)
{
    if (InputFlat.Num() != SampleCount * InputSize || TargetFlat.Num() != SampleCount * OutputSize || SampleCount <= 0)
    {
        UE_LOG(LogDensityFunctionTrainer, Warning, TEXT("Invalid training data layout."));
        return;
    }

    TArray<float> HiddenOutputs;
    TArray<float> OutputValues;

    for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 Sample = 0; Sample < SampleCount; ++Sample)
        {
            const int32 InputStart = Sample * InputSize;
            const int32 TargetStart = Sample * OutputSize;

            TArray<float> CurrentInput;
            CurrentInput.SetNum(InputSize);
            for (int32 InputIdx = 0; InputIdx < InputSize; ++InputIdx)
            {
                CurrentInput[InputIdx] = InputFlat[InputStart + InputIdx];
            }

            ForwardPass(CurrentInput, HiddenOutputs, OutputValues);

            TArray<float> OutputError;
            OutputError.Init(0.f, OutputSize);

            for (int32 OutputIdx = 0; OutputIdx < OutputSize; ++OutputIdx)
            {
                const float Target = TargetFlat[TargetStart + OutputIdx];
                const float Error = Target - OutputValues[OutputIdx];
                OutputError[OutputIdx] = Error * SigmoidDerivative(OutputValues[OutputIdx]);
                EpochError += FMath::Abs(Error);
            }

            TArray<float> HiddenError;
            HiddenError.Init(0.f, HiddenSize);
            for (int32 HiddenIdx = 0; HiddenIdx < HiddenSize; ++HiddenIdx)
            {
                float Sum = 0.f;
                for (int32 OutputIdx = 0; OutputIdx < OutputSize; ++OutputIdx)
                {
                    Sum += HiddenOutputWeights[HiddenIdx * OutputSize + OutputIdx] * OutputError[OutputIdx];
                }
                HiddenError[HiddenIdx] = Sum * SigmoidDerivative(HiddenOutputs[HiddenIdx]);
            }

            for (int32 OutputIdx = 0; OutputIdx < OutputSize; ++OutputIdx)
            {
                for (int32 HiddenIdx = 0; HiddenIdx < HiddenSize; ++HiddenIdx)
                {
                    const int32 WeightIndex = HiddenIdx * OutputSize + OutputIdx;
                    HiddenOutputWeights[WeightIndex] += LearningRate * OutputError[OutputIdx] * HiddenOutputs[HiddenIdx];
                }
                OutputBiases[OutputIdx] += LearningRate * OutputError[OutputIdx];
            }

            for (int32 HiddenIdx = 0; HiddenIdx < HiddenSize; ++HiddenIdx)
            {
                for (int32 InputIdx = 0; InputIdx < InputSize; ++InputIdx)
                {
                    const int32 WeightIndex = InputIdx * HiddenSize + HiddenIdx;
                    InputHiddenWeights[WeightIndex] += LearningRate * HiddenError[HiddenIdx] * CurrentInput[InputIdx];
                }
                HiddenBiases[HiddenIdx] += LearningRate * HiddenError[HiddenIdx];
            }
        }

        if (Epoch % 100 == 0)
        {
            UE_LOG(LogDensityFunctionTrainer, Log, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, Epochs, EpochError);
        }
    }
}

TArray<float> UDensityFunctionTrainer::Predict(const TArray<float>& Input) const
{
    if (Input.Num() != InputSize)
    {
        UE_LOG(LogDensityFunctionTrainer, Warning, TEXT("Predict input size mismatch."));
        return {};
    }

    TArray<float> HiddenOutputs;
    TArray<float> OutputValues;
    ForwardPass(Input, HiddenOutputs, OutputValues);
    return OutputValues;
}

void UDensityFunctionTrainer::RunExampleTraining()
{
    TArray<float> Inputs = {0.f, 1.f, 2.f};
    TArray<float> Targets = {1.f, 0.5f, 0.2f};
    TrainNetwork(Inputs, Targets, 3, 1000, 0.01f);
    const TArray<float> Prediction = Predict({1.5f});
    if (Prediction.Num() > 0)
    {
        UE_LOG(LogDensityFunctionTrainer, Log, TEXT("Predicted Density: %.4f"), Prediction[0]);
    }
}
