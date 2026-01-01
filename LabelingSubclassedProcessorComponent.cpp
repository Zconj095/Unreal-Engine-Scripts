#include "LabelingSubclassedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingSubclassedProcessorComponent::ULabelingSubclassedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingSubclassedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign subclass and class labels from demo probs
    const FSubclassAndClassLabels Assigned = AssignSubclassedLabelsFlat(
        DemoSubclassProbFlat, DemoRows, DemoNumSubclasses,
        DemoClassProbFlat, DemoNumClasses);

    FString SubStr, ClsStr;
    for (int32 i = 0; i < Assigned.SubclassLabels.Num(); ++i)
    {
        SubStr += FString::FromInt(Assigned.SubclassLabels[i]);
        if (i + 1 < Assigned.SubclassLabels.Num()) SubStr += TEXT(", ");
    }
    for (int32 i = 0; i < Assigned.ClassLabels.Num(); ++i)
    {
        ClsStr += FString::FromInt(Assigned.ClassLabels[i]);
        if (i + 1 < Assigned.ClassLabels.Num()) ClsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Subclass Labels: %s"), *SubStr);
    UE_LOG(LogTemp, Log, TEXT("Class Labels: %s"), *ClsStr);

    // Train tiny neural example
    TrainSubclassedLabelingNeuralNetwork();
}

FSubclassAndClassLabels ULabelingSubclassedProcessorComponent::AssignSubclassedLabelsFlat(const TArray<float>& SubclassProbFlat, int32 Rows, int32 NumSubclasses,
                                                                                           const TArray<float>& ClassProbFlat, int32 NumClasses)
{
    FSubclassAndClassLabels Out;
    if (Rows <= 0 || NumSubclasses <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignSubclassedLabelsFlat: invalid dims."));
        return Out;
    }
    if (SubclassProbFlat.Num() != Rows * NumSubclasses)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignSubclassedLabelsFlat: subclass size %d != Rows*NumSubclasses %d."), SubclassProbFlat.Num(), Rows * NumSubclasses);
        return Out;
    }
    if (ClassProbFlat.Num() != NumSubclasses * NumClasses)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignSubclassedLabelsFlat: class size %d != NumSubclasses*NumClasses %d."), ClassProbFlat.Num(), NumSubclasses * NumClasses);
        return Out;
    }

    Out.SubclassLabels.SetNum(Rows);
    Out.ClassLabels.SetNum(Rows);

    for (int32 r = 0; r < Rows; ++r)
    {
        const float* SubRow = SubclassProbFlat.GetData() + r * NumSubclasses;
        const int32 BestSubclass = ArgMax(SubRow, NumSubclasses);
        Out.SubclassLabels[r] = BestSubclass;

        const float* ClassRow = ClassProbFlat.GetData() + BestSubclass * NumClasses;
        const int32 BestClass = ArgMax(ClassRow, NumClasses);
        Out.ClassLabels[r] = BestClass;
    }

    return Out;
}

int32 ULabelingSubclassedProcessorComponent::ArgMax(const float* Data, int32 Count)
{
    if (Count <= 0) return 0;
    int32 BestIdx = 0;
    float BestVal = Data[0];
    for (int32 i = 1; i < Count; ++i)
    {
        if (Data[i] > BestVal)
        {
            BestVal = Data[i];
            BestIdx = i;
        }
    }
    return BestIdx;
}

// -------------------- Simple 4-6-3 MLP --------------------

ULabelingSubclassedProcessorComponent::FMLP463::FMLP463()
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

float ULabelingSubclassedProcessorComponent::FMLP463::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingSubclassedProcessorComponent::FMLP463::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingSubclassedProcessorComponent::FMLP463::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingSubclassedProcessorComponent::FMLP463::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingSubclassedProcessorComponent::TrainSubclassedLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 4 features; Outputs: 3 one-hot slots (toy)
    const float Inputs[3][4] = {
        { 0.5f, 0.3f, 0.2f, 0.1f },
        { 0.7f, 0.6f, 0.5f, 0.4f },
        { 0.9f, 0.8f, 0.7f, 0.6f }
    };
    const float Targets[3][3] = {
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f }
    };
    const int32 SampleCount = 3;

    FMLP463 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP463::HiddenSize];
            float Output[FMLP463::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP463::OutSize; ++o)
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

