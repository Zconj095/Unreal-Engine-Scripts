#include "LabelingCategorizedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingCategorizedProcessorComponent::ULabelingCategorizedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingCategorizedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign category labels to data points using a threshold function
    const TArray<int32> CategorizedLabels = AssignCategorizedLabelsThreshold(DemoData, Threshold, BelowThresholdLabel, AboveEqualThresholdLabel);
    FString LabelsStr;
    for (int32 i = 0; i < CategorizedLabels.Num(); ++i)
    {
        LabelsStr += FString::FromInt(CategorizedLabels[i]);
        if (i + 1 < CategorizedLabels.Num()) LabelsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Categorized Labels: %s"), *LabelsStr);

    // Group-level categorization (mode)
    const int32 GroupCategory = DetermineGroupCategory(CategorizedLabels);
    UE_LOG(LogTemp, Log, TEXT("Group-Level Category: %d"), GroupCategory);

    // Train demo neural classifier
    TrainCategorizedLabelingNeuralNetwork();
}

TArray<int32> ULabelingCategorizedProcessorComponent::AssignCategorizedLabelsThreshold(const TArray<float>& Data,
                                                                                        float InThreshold,
                                                                                        int32 BelowLabel,
                                                                                        int32 AboveEqualLabel)
{
    TArray<int32> Labels;
    Labels.SetNum(Data.Num());
    for (int32 i = 0; i < Data.Num(); ++i)
    {
        Labels[i] = (Data[i] < InThreshold) ? BelowLabel : AboveEqualLabel;
    }
    return Labels;
}

int32 ULabelingCategorizedProcessorComponent::DetermineGroupCategory(const TArray<int32>& GroupLabels)
{
    if (GroupLabels.Num() == 0)
    {
        return 0;
    }

    TMap<int32, int32> Counts;
    for (int32 L : GroupLabels)
    {
        int32& C = Counts.FindOrAdd(L);
        ++C;
    }

    int32 BestLabel = GroupLabels[0];
    int32 BestCount = -1;
    for (const TPair<int32, int32>& Pair : Counts)
    {
        if (Pair.Value > BestCount)
        {
            BestCount = Pair.Value;
            BestLabel = Pair.Key;
        }
    }
    return BestLabel;
}

// -------------------- Simple 2-5-2 MLP --------------------

ULabelingCategorizedProcessorComponent::FMLP2C::FMLP2C()
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

float ULabelingCategorizedProcessorComponent::FMLP2C::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingCategorizedProcessorComponent::FMLP2C::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingCategorizedProcessorComponent::FMLP2C::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

    for (int32 o = 0; o < OutSize; ++o)
    {
        float Z = B2[o];
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            Z += W2[o][h] * OutHidden[h];
        }
        OutOutput[o] = Sigmoid(Z);
    }
}

void ULabelingCategorizedProcessorComponent::FMLP2C::TrainSGD(const float* Input, const float* Target, float LearningRate)
{
    float Hidden[HiddenSize];
    float Output[OutSize];
    Forward(Input, Hidden, Output);

    // Output layer deltas for each class
    float DeltaOut[OutSize];
    for (int32 o = 0; o < OutSize; ++o)
    {
        const float Err = Output[o] - Target[o];
        DeltaOut[o] = Err * SigmoidDerivFromY(Output[o]);
    }

    // Hidden layer deltas
    float DeltaHidden[HiddenSize] = {0};
    for (int32 h = 0; h < HiddenSize; ++h)
    {
        float Grad = 0.0f;
        for (int32 o = 0; o < OutSize; ++o)
        {
            Grad += W2[o][h] * DeltaOut[o];
        }
        DeltaHidden[h] = Grad * SigmoidDerivFromY(Hidden[h]);
    }

    // Update W2, B2
    for (int32 o = 0; o < OutSize; ++o)
    {
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            W2[o][h] -= LearningRate * DeltaOut[o] * Hidden[h];
        }
        B2[o] -= LearningRate * DeltaOut[o];
    }

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

void ULabelingCategorizedProcessorComponent::TrainCategorizedLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs (2D) and one-hot targets (2 classes) per original example
    const float Inputs[3][2] = {
        { 0.5f, 0.3f },
        { 1.0f, 0.7f },
        { 1.5f, 1.2f }
    };
    const float Targets[3][2] = {
        { 1.f, 0.f }, // Category 0
        { 0.f, 1.f }, // Category 1
        { 1.f, 0.f }  // Category 0
    };
    const int32 SampleCount = 3;

    FMLP2C Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP2C::HiddenSize];
            float Output[FMLP2C::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            // MSE across outputs
            for (int32 o = 0; o < FMLP2C::OutSize; ++o)
            {
                const double Diff = static_cast<double>(Output[o]) - static_cast<double>(Targets[s][o]);
                EpochError += 0.5 * Diff * Diff;
            }
        }

        EpochError /= static_cast<double>(SampleCount);
        UE_LOG(LogTemp, Log, TEXT("Epoch %d, Error: %f"), Epoch, EpochError);
        if (EpochError < ErrorThreshold)
        {
            break;
        }
    }
}

