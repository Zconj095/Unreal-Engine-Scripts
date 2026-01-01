#include "CrystalMagicalFieldPredictor.h"

DEFINE_LOG_CATEGORY(LogCrystalMagicalFieldPredictor);

UCrystalMagicalFieldPredictor::UCrystalMagicalFieldPredictor()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCrystalMagicalFieldPredictor::BeginPlay()
{
    Super::BeginPlay();

    InitializeNetwork();
    RunExampleTraining();
}

void UCrystalMagicalFieldPredictor::InitializeNetwork()
{
    InputHiddenWeights.SetNumZeroed(InputSize * HiddenSize);
    HiddenOutputWeights.SetNumZeroed(HiddenSize * OutputSize);
    HiddenBiases.SetNumZeroed(HiddenSize);
    OutputBiases.SetNumZeroed(OutputSize);

    for (int32 Index = 0; Index < InputHiddenWeights.Num(); ++Index)
    {
        InputHiddenWeights[Index] = FMath::FRandRange(-1.f, 1.f);
    }

    for (int32 Index = 0; Index < HiddenOutputWeights.Num(); ++Index)
    {
        HiddenOutputWeights[Index] = FMath::FRandRange(-1.f, 1.f);
    }

    for (int32 Index = 0; Index < HiddenBiases.Num(); ++Index)
    {
        HiddenBiases[Index] = FMath::FRandRange(-0.5f, 0.5f);
    }

    for (int32 Index = 0; Index < OutputBiases.Num(); ++Index)
    {
        OutputBiases[Index] = FMath::FRandRange(-0.5f, 0.5f);
    }
}

float UCrystalMagicalFieldPredictor::Sigmoid(float Value) const
{
    return 1.f / (1.f + FMath::Exp(-Value));
}

float UCrystalMagicalFieldPredictor::SigmoidDerivative(float Value) const
{
    return Value * (1.f - Value);
}

void UCrystalMagicalFieldPredictor::ForwardPass(const TArray<float>& Input, TArray<float>& HiddenOutputs, TArray<float>& OutputValues) const
{
    HiddenOutputs.Init(0.f, HiddenSize);
    OutputValues.Init(0.f, OutputSize);

    for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
    {
        float Sum = HiddenBiases[HiddenIndex];
        for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
        {
            Sum += InputHiddenWeights[InputIndex * HiddenSize + HiddenIndex] * Input[InputIndex];
        }
        HiddenOutputs[HiddenIndex] = Sigmoid(Sum);
    }

    for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
    {
        float Sum = OutputBiases[OutputIndex];
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
        {
            Sum += HiddenOutputWeights[HiddenIndex * OutputSize + OutputIndex] * HiddenOutputs[HiddenIndex];
        }
        OutputValues[OutputIndex] = Sigmoid(Sum);
    }
}

void UCrystalMagicalFieldPredictor::TrainNetwork(const TArray<float>& InputFlat, const TArray<float>& TargetFlat, int32 SampleCount, int32 Epochs, float LearningRate)
{
    if (InputFlat.Num() != SampleCount * InputSize ||
        TargetFlat.Num() != SampleCount * OutputSize ||
        SampleCount <= 0)
    {
        UE_LOG(LogCrystalMagicalFieldPredictor, Warning, TEXT("Invalid training data layout."));
        return;
    }

    TArray<float> HiddenOutputs;
    TArray<float> OutputValues;

    for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 SampleIndex = 0; SampleIndex < SampleCount; ++SampleIndex)
        {
            const int32 InputStart = SampleIndex * InputSize;
            const int32 TargetStart = SampleIndex * OutputSize;

            TArray<float> CurrentInput;
            CurrentInput.SetNum(InputSize);
            for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
            {
                CurrentInput[InputIndex] = InputFlat[InputStart + InputIndex];
            }
            ForwardPass(CurrentInput, HiddenOutputs, OutputValues);

            TArray<float> OutputError;
            OutputError.Init(0.f, OutputSize);
            for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
            {
                const float Target = TargetFlat[TargetStart + OutputIndex];
                const float Error = Target - OutputValues[OutputIndex];
                OutputError[OutputIndex] = Error * SigmoidDerivative(OutputValues[OutputIndex]);
                EpochError += FMath::Abs(Error);
            }

            TArray<float> HiddenError;
            HiddenError.Init(0.f, HiddenSize);

            for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
            {
                float Sum = 0.f;
                for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
                {
                    Sum += HiddenOutputWeights[HiddenIndex * OutputSize + OutputIndex] * OutputError[OutputIndex];
                }
                HiddenError[HiddenIndex] = Sum * SigmoidDerivative(HiddenOutputs[HiddenIndex]);
            }

            for (int32 OutputIndex = 0; OutputIndex < OutputSize; ++OutputIndex)
            {
                for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
                {
                    const int32 WeightIndex = HiddenIndex * OutputSize + OutputIndex;
                    HiddenOutputWeights[WeightIndex] += LearningRate * OutputError[OutputIndex] * HiddenOutputs[HiddenIndex];
                }
                OutputBiases[OutputIndex] += LearningRate * OutputError[OutputIndex];
            }

            for (int32 HiddenIndex = 0; HiddenIndex < HiddenSize; ++HiddenIndex)
            {
                for (int32 InputIndex = 0; InputIndex < InputSize; ++InputIndex)
                {
                    const int32 WeightIndex = InputIndex * HiddenSize + HiddenIndex;
                    InputHiddenWeights[WeightIndex] += LearningRate * HiddenError[HiddenIndex] * CurrentInput[InputIndex];
                }
                HiddenBiases[HiddenIndex] += LearningRate * HiddenError[HiddenIndex];
            }
        }

        if (Epoch % 100 == 0)
        {
            UE_LOG(LogCrystalMagicalFieldPredictor, Log, TEXT("Epoch %d/%d, Error: %.6f"), Epoch + 1, Epochs, EpochError);
        }
    }
}

TArray<float> UCrystalMagicalFieldPredictor::Predict(const TArray<float>& Input) const
{
    if (Input.Num() != InputSize)
    {
    #if UE_BUILD_DEBUG
        UE_LOG(LogCrystalMagicalFieldPredictor, Warning, TEXT("Predict input size mismatch."));
    #endif
        return {};
    }

    TArray<float> HiddenOutputs;
    TArray<float> OutputValues;
    ForwardPass(Input, HiddenOutputs, OutputValues);
    return OutputValues;
}

void UCrystalMagicalFieldPredictor::RunExampleTraining()
{
    TArray<float> Inputs = {
        0.1f, 0.2f,
        0.3f, 0.4f
    };

    TArray<float> Targets = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f
    };

    TrainNetwork(Inputs, Targets, 2, 1000, 0.01f);
    const TArray<float> Prediction = Predict({0.15f, 0.25f});
    FString Output = TEXT("Predicted Vector: ");
    for (float Value : Prediction)
    {
        Output += FString::Printf(TEXT("%.4f "), Value);
    }
    UE_LOG(LogCrystalMagicalFieldPredictor, Log, TEXT("%s"), *Output);
}
