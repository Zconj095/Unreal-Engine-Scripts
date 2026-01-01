#include "LabelingClassedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingClassedProcessorComponent::ULabelingClassedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingClassedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Flat class labels via threshold rule
    const TArray<int32> ClassLabels = AssignClassLabelsThreshold(DemoData, ClassThreshold, BelowClassLabel, AboveEqualClassLabel);
    FString FlatStr;
    for (int32 i = 0; i < ClassLabels.Num(); ++i)
    {
        FlatStr += FString::FromInt(ClassLabels[i]);
        if (i + 1 < ClassLabels.Num()) FlatStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Flat Class Labels: %s"), *FlatStr);

    // Hierarchical labels via threshold + modulo rule
    const TArray<FString> HierLabels = AssignHierarchicalClassLabelsThresholdMod(DemoData, ClassThreshold, BelowClassLabel, AboveEqualClassLabel, SubScale, SubModulo, SubOffset);
    FString HierStr;
    for (int32 i = 0; i < HierLabels.Num(); ++i)
    {
        HierStr += HierLabels[i];
        if (i + 1 < HierLabels.Num()) HierStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Hierarchical Class Labels: %s"), *HierStr);

    // Train demo neural classifier
    TrainClassLabelingNeuralNetwork();
}

TArray<int32> ULabelingClassedProcessorComponent::AssignClassLabelsThreshold(const TArray<float>& Data,
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

TArray<FString> ULabelingClassedProcessorComponent::AssignHierarchicalClassLabelsThresholdMod(const TArray<float>& Data,
                                                                                               float InThreshold,
                                                                                               int32 BelowLabel,
                                                                                               int32 AboveEqualLabel,
                                                                                               float InSubScale,
                                                                                               int32 InSubModulo,
                                                                                               int32 InSubOffset)
{
    TArray<FString> Out;
    Out.SetNum(Data.Num());
    const int32 SafeModulo = (InSubModulo == 0) ? 1 : InSubModulo;

    for (int32 i = 0; i < Data.Num(); ++i)
    {
        const int32 ClassLabel = (Data[i] < InThreshold) ? BelowLabel : AboveEqualLabel;
        const int32 SubBase = static_cast<int32>(FMath::FloorToInt(Data[i] * InSubScale)) + InSubOffset;
        const int32 SubLabel = (SafeModulo != 0) ? (SubBase % SafeModulo) : 0;
        Out[i] = FString::Printf(TEXT("Class-%d_SubClass-%d"), ClassLabel, SubLabel);
    }
    return Out;
}

// -------------------- Simple 1-5-2 MLP --------------------

ULabelingClassedProcessorComponent::FMLP1C::FMLP1C()
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

float ULabelingClassedProcessorComponent::FMLP1C::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingClassedProcessorComponent::FMLP1C::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingClassedProcessorComponent::FMLP1C::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingClassedProcessorComponent::FMLP1C::TrainSGD(const float* Input, const float* Target, float LearningRate)
{
    float Hidden[HiddenSize];
    float Output[OutSize];
    Forward(Input, Hidden, Output);

    float DeltaOut[OutSize];
    for (int32 o = 0; o < OutSize; ++o)
    {
        const float Err = Output[o] - Target[o];
        DeltaOut[o] = Err * SigmoidDerivFromY(Output[o]);
    }

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

    for (int32 o = 0; o < OutSize; ++o)
    {
        for (int32 h = 0; h < HiddenSize; ++h)
        {
            W2[o][h] -= LearningRate * DeltaOut[o] * Hidden[h];
        }
        B2[o] -= LearningRate * DeltaOut[o];
    }

    for (int32 h = 0; h < HiddenSize; ++h)
    {
        for (int32 i = 0; i < InSize; ++i)
        {
            W1[h][i] -= LearningRate * DeltaHidden[h] * Input[i];
        }
        B1[h] -= LearningRate * DeltaHidden[h];
    }
}

void ULabelingClassedProcessorComponent::TrainClassLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: scalar x; Outputs: one-hot [C0, C1]
    const float Inputs[3][1] = {
        { 0.5f },
        { 1.0f },
        { 1.5f }
    };
    const float Targets[3][2] = {
        { 1.f, 0.f }, // Class 0
        { 0.f, 1.f }, // Class 1
        { 1.f, 0.f }  // Class 0
    };
    const int32 SampleCount = 3;

    FMLP1C Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP1C::HiddenSize];
            float Output[FMLP1C::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP1C::OutSize; ++o)
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

