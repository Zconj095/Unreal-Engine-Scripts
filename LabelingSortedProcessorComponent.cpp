#include "LabelingSortedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingSortedProcessorComponent::ULabelingSortedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingSortedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign labels to sorted data using alpha*value + beta*rank
    const TArray<float> Labels = AssignLabelsToSortedDataRanked(DemoData, LabelAlpha, LabelBeta);
    FString LabelStr;
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
        LabelStr += FString::SanitizeFloat(Labels[i]);
        if (i + 1 < Labels.Num()) LabelStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels for Sorted Data: %s"), *LabelStr);

    // Train tiny neural model
    TrainLabelingNeuralNetwork();
}

TArray<float> ULabelingSortedProcessorComponent::AssignLabelsToSortedDataRanked(const TArray<float>& Data, float Alpha, float Beta)
{
    const int32 N = Data.Num();
    TArray<float> Labels;
    Labels.SetNum(N);

    // Build pairs (Value, OriginalIndex)
    struct FPair { float Value; int32 Index; };
    TArray<FPair> Pairs;
    Pairs.SetNumUninitialized(N);
    for (int32 i = 0; i < N; ++i)
    {
        Pairs[i] = { Data[i], i };
    }

    // Sort ascending by value
    Pairs.Sort([](const FPair& A, const FPair& B){ return A.Value < B.Value; });

    // Assign labels based on rank (1-based)
    for (int32 i = 0; i < N; ++i)
    {
        const int32 Rank = i + 1;
        const float L = Alpha * Pairs[i].Value + Beta * static_cast<float>(Rank);
        Labels[Pairs[i].Index] = L;
    }

    return Labels;
}

// -------------------- Simple 2-5-1 MLP --------------------

ULabelingSortedProcessorComponent::FMLP251::FMLP251()
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

float ULabelingSortedProcessorComponent::FMLP251::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingSortedProcessorComponent::FMLP251::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingSortedProcessorComponent::FMLP251::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

    float Z = B2[0];
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        Z += W2[0][h] * OutHidden[h];
    }
    OutOutput[0] = Sigmoid(Z);
}

void ULabelingSortedProcessorComponent::FMLP251::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingSortedProcessorComponent::TrainLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: [Value, Rank], Outputs: target label
    const float Inputs[3][2] = {
        { 1.0f, 1.0f },
        { 2.0f, 2.0f },
        { 3.0f, 3.0f }
    };
    const float Targets[3] = { 1.5f, 2.5f, 3.5f };
    const int32 SampleCount = 3;

    FMLP251 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            const float T = Targets[s];
            Net.TrainSGD(Inputs[s], &T, LearningRate);

            float Hidden[FMLP251::HiddenSize];
            float Output[FMLP251::OutSize];
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

