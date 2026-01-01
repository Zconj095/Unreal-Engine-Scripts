#include "InstructionAdherenceProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

UInstructionAdherenceProcessorComponent::UInstructionAdherenceProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInstructionAdherenceProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Demo: Instruction Adherence
    const float Ia = CalculateInstructionAdherence(ComprehensionLevels, ExecutionAccuracies);
    UE_LOG(LogTemp, Log, TEXT("Instruction Adherence Score (Ia): %f"), Ia);

    // Demo: Comprehension Level
    const float Cj = CalculateComprehensionLevel(AttentionLevel, Thoroughness, InstructionComplexity);
    UE_LOG(LogTemp, Log, TEXT("Comprehension Level (Cj): %f"), Cj);

    // Demo: tiny neural model training
    TrainNeuralAdherenceModel();
}

float UInstructionAdherenceProcessorComponent::CalculateInstructionAdherence(
    const TArray<float>& InComprehensionLevels,
    const TArray<float>& InExecutionAccuracies)
{
    if (InComprehensionLevels.Num() != InExecutionAccuracies.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Comprehension levels and execution accuracies must have the same length."));
        return 0.0f;
    }
    if (InComprehensionLevels.Num() == 0)
    {
        return 0.0f;
    }

    double Sum = 0.0;
    for (int32 i = 0; i < InComprehensionLevels.Num(); ++i)
    {
        Sum += static_cast<double>(InComprehensionLevels[i]) * static_cast<double>(InExecutionAccuracies[i]);
    }
    return static_cast<float>(Sum / static_cast<double>(InComprehensionLevels.Num()));
}

float UInstructionAdherenceProcessorComponent::CalculateComprehensionLevel(float InAttentionLevel, float InThoroughness, float InInstructionComplexity)
{
    return InAttentionLevel * InThoroughness / (1.0f + InInstructionComplexity);
}

// -------------------- Simple MLP --------------------

UInstructionAdherenceProcessorComponent::FMLP::FMLP()
{
    const float Scale = 0.1f;
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h][i] = FMath::FRandRange(-Scale, Scale);
        }
        B1[h] = FMath::FRandRange(-Scale, Scale);
    }
    for (int32 o = 0; o < OutSize; ++o)
    {
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            W2[o][h] = FMath::FRandRange(-Scale, Scale);
        }
        B2[o] = FMath::FRandRange(-Scale, Scale);
    }
}

float UInstructionAdherenceProcessorComponent::FMLP::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float UInstructionAdherenceProcessorComponent::FMLP::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void UInstructionAdherenceProcessorComponent::FMLP::Forward(const float* Input, float* OutHidden, float* OutOutput) const
{
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        float Z = B1[h];
        for (int32 i = 0; i < InSize; ++i)
        {
            Z += W1[h][i] * Input[i];
        }
        OutHidden[h] = Sigmoid(Z);
    }

    float Zout = B2[0];
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        Zout += W2[0][h] * OutHidden[h];
    }
    OutOutput[0] = Sigmoid(Zout);
}

void UInstructionAdherenceProcessorComponent::FMLP::TrainSGD(const float* Input, const float* Target, float LearningRate)
{
    float Hidden[HiddenSize];
    float Output[OutSize];
    Forward(Input, Hidden, Output);

    const float Error = Output[0] - Target[0];
    const float DeltaOut = Error * SigmoidDerivFromY(Output[0]);

    float DeltaHidden[HiddenSize];
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        const float Grad = W2[0][h] * DeltaOut;
        DeltaHidden[h] = Grad * SigmoidDerivFromY(Hidden[h]);
    }

    for (int32 h = 0; h < HiddenSize; ++h)
    {
        W2[0][h] -= LearningRate * DeltaOut * Hidden[h];
    }
    B2[0] -= LearningRate * DeltaOut;

    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h][i] -= LearningRate * DeltaHidden[h] * Input[i];
        }
        B1[h] -= LearningRate * DeltaHidden[h];
    }
}

void UInstructionAdherenceProcessorComponent::TrainNeuralAdherenceModel(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: [Attention, Thoroughness, Complexity]; Target: Comprehension level
    const float Inputs[3][3] = {
        { 0.8f, 0.9f, 0.3f },
        { 0.7f, 0.8f, 0.5f },
        { 0.6f, 0.7f, 0.7f }
    };
    const float Targets[3] = { 0.7f, 0.65f, 0.55f };
    const int32 SampleCount = 3;

    FMLP Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            const float T = Targets[s];
            Net.TrainSGD(Inputs[s], &T, LearningRate);

            float Hidden[FMLP::HiddenSize];
            float Output[FMLP::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            const double Diff = static_cast<double>(Output[0]) - static_cast<double>(T);
            EpochError += 0.5 * Diff * Diff;
        }

        EpochError /= static_cast<double>(SampleCount);
        UE_LOG(LogTemp, Log, TEXT("Epoch %d, Error: %f"), Epoch, EpochError);
        if (EpochError < ErrorThreshold)
        {
            break;
        }
    }
}

