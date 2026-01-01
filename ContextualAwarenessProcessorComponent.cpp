#include "ContextualAwarenessProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

UContextualAwarenessProcessorComponent::UContextualAwarenessProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UContextualAwarenessProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Demo contextual function: feature * context
    const float CA = CalculateContextualAwareness(Features, Weights, Context);
    UE_LOG(LogTemp, Log, TEXT("Contextual Awareness Score (CA): %f"), CA);

    // Demo similarity function: exp(-abs(feature - context))
    const float Sc = CalculateContextSimilarity(Features, Context);
    UE_LOG(LogTemp, Log, TEXT("Context Similarity (Sc): %f"), Sc);

    // Demo tiny neural training
    TrainContextualAwarenessNeuralNetwork();
}

float UContextualAwarenessProcessorComponent::CalculateContextualAwareness(
    const TArray<float>& InFeatures,
    const TArray<float>& InWeights,
    const TArray<float>& InContext)
{
    if (!(InFeatures.Num() == InWeights.Num() && InFeatures.Num() == InContext.Num()))
    {
        UE_LOG(LogTemp, Error, TEXT("Features, weights, and context arrays must have the same length."));
        return 0.0f;
    }

    float Score = 0.0f;
    for (int32 i = 0; i < InFeatures.Num(); ++i)
    {
        Score += InWeights[i] * (InFeatures[i] * InContext[i]);
    }
    return Score;
}

float UContextualAwarenessProcessorComponent::CalculateContextSimilarity(
    const TArray<float>& InFeatures,
    const TArray<float>& InContext)
{
    if (InFeatures.Num() != InContext.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Features and context arrays must have the same length."));
        return 0.0f;
    }

    if (InFeatures.Num() == 0)
    {
        return 0.0f;
    }

    double Sum = 0.0;
    for (int32 i = 0; i < InFeatures.Num(); ++i)
    {
        const double Diff = FMath::Abs((double)InFeatures[i] - (double)InContext[i]);
        Sum += FMath::Exp(-Diff);
    }

    return static_cast<float>(Sum / static_cast<double>(InFeatures.Num()));
}

// -------------------- Simple MLP --------------------

UContextualAwarenessProcessorComponent::FMLP::FMLP()
{
    // Initialize with small random weights
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

float UContextualAwarenessProcessorComponent::FMLP::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float UContextualAwarenessProcessorComponent::FMLP::SigmoidDerivFromY(float Y)
{
    // derivative given output Y = sigmoid(X) -> Y*(1-Y)
    return Y * (1.0f - Y);
}

void UContextualAwarenessProcessorComponent::FMLP::Forward(const float* Input, float* OutHidden, float* OutOutput) const
{
    // Hidden layer
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        float Z = B1[h];
        for (int32 i = 0; i < InSize; ++i)
        {
            Z += W1[h][i] * Input[i];
        }
        OutHidden[h] = Sigmoid(Z);
    }

    // Output layer (single output)
    float Zout = B2[0];
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        Zout += W2[0][h] * OutHidden[h];
    }
    OutOutput[0] = Sigmoid(Zout);
}

void UContextualAwarenessProcessorComponent::FMLP::TrainSGD(const float* Input, const float* Target, float LearningRate)
{
    float Hidden[HiddenSize];
    float Output[OutSize];
    Forward(Input, Hidden, Output);

    // Output layer error (MSE) and deltas
    float Error = Output[0] - Target[0];
    float DeltaOut = Error * SigmoidDerivFromY(Output[0]);

    // Hidden layer deltas
    float DeltaHidden[HiddenSize];
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        float Grad = W2[0][h] * DeltaOut;
        DeltaHidden[h] = Grad * SigmoidDerivFromY(Hidden[h]);
    }

    // Update W2, B2
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        W2[0][h] -= LearningRate * DeltaOut * Hidden[h];
    }
    B2[0] -= LearningRate * DeltaOut;

    // Update W1, B1
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h][i] -= LearningRate * DeltaHidden[h] * Input[i];
        }
        B1[h] -= LearningRate * DeltaHidden[h];
    }
}

void UContextualAwarenessProcessorComponent::TrainContextualAwarenessNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Training dataset similar to the Unity example
    const float Inputs[3][3] = {
        { 1.0f, 0.5f, 0.2f },
        { 0.8f, 0.3f, 0.6f },
        { 0.2f, 0.9f, 0.4f }
    };
    const float Targets[3] = { 0.7f, 0.65f, 0.55f };
    const int32 SampleCount = 3;

    FMLP Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        // One epoch of SGD over samples
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            const float Target = Targets[s];
            Net.TrainSGD(Inputs[s], &Target, LearningRate);

            float Hidden[FMLP::HiddenSize];
            float Output[FMLP::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            const double Diff = static_cast<double>(Output[0]) - static_cast<double>(Target);
            EpochError += 0.5 * Diff * Diff; // MSE contribution
        }

        EpochError /= static_cast<double>(SampleCount);
        UE_LOG(LogTemp, Log, TEXT("Epoch %d, Error: %f"), Epoch, EpochError);
        if (EpochError < ErrorThreshold)
        {
            break;
        }
    }
}

