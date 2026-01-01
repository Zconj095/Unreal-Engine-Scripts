#include "LabelingSubtypesProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingSubtypesProcessorComponent::ULabelingSubtypesProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingSubtypesProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    const FSubtypeLabelsResult Res = AssignSubtypeLabelsFlat(
        DemoSubtypeProbFlat, DemoRows, DemoNumSubtypes, DemoTypeLabels);

    FString SubtypesStr, FullStr;
    for (int32 i = 0; i < Res.SubtypeLabels.Num(); ++i)
    {
        SubtypesStr += FString::FromInt(Res.SubtypeLabels[i]);
        if (i + 1 < Res.SubtypeLabels.Num()) SubtypesStr += TEXT(", ");
    }
    for (int32 i = 0; i < Res.FullLabels.Num(); ++i)
    {
        FullStr += Res.FullLabels[i];
        if (i + 1 < Res.FullLabels.Num()) FullStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Subtype Labels: %s"), *SubtypesStr);
    UE_LOG(LogTemp, Log, TEXT("Full Labels: %s"), *FullStr);

    TrainSubtypeLabelingNeuralNetwork();
}

FSubtypeLabelsResult ULabelingSubtypesProcessorComponent::AssignSubtypeLabelsFlat(const TArray<float>& SubtypeProbFlat, int32 Rows, int32 NumSubtypes,
                                                                                  const TArray<int32>& TypeLabels)
{
    FSubtypeLabelsResult Out;
    if (Rows <= 0 || NumSubtypes <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignSubtypeLabelsFlat: invalid dims."));
        return Out;
    }
    if (SubtypeProbFlat.Num() != Rows * NumSubtypes)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignSubtypeLabelsFlat: size %d != Rows*NumSubtypes %d."), SubtypeProbFlat.Num(), Rows * NumSubtypes);
        return Out;
    }
    if (TypeLabels.Num() != Rows)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignSubtypeLabelsFlat: TypeLabels size %d != Rows %d."), TypeLabels.Num(), Rows);
        return Out;
    }

    Out.SubtypeLabels.SetNum(Rows);
    Out.FullLabels.SetNum(Rows);

    for (int32 r = 0; r < Rows; ++r)
    {
        const float* RowPtr = SubtypeProbFlat.GetData() + r * NumSubtypes;
        const int32 BestSubtype = ArgMax(RowPtr, NumSubtypes);
        Out.SubtypeLabels[r] = BestSubtype;
        Out.FullLabels[r] = FString::Printf(TEXT("Type-%d_Subtype-%d"), TypeLabels[r], BestSubtype);
    }
    return Out;
}

int32 ULabelingSubtypesProcessorComponent::ArgMax(const float* Data, int32 Count)
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

// -------------------- Simple 3-5-3 MLP --------------------

ULabelingSubtypesProcessorComponent::FMLP353::FMLP353()
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

float ULabelingSubtypesProcessorComponent::FMLP353::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingSubtypesProcessorComponent::FMLP353::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingSubtypesProcessorComponent::FMLP353::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingSubtypesProcessorComponent::FMLP353::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingSubtypesProcessorComponent::TrainSubtypeLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 3 features; Outputs: one-hot 3 subtypes
    const float Inputs[3][3] = {
        { 0.5f, 0.3f, 0.2f },
        { 0.7f, 0.6f, 0.5f },
        { 0.9f, 0.8f, 0.7f }
    };
    const float Targets[3][3] = {
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f }
    };
    const int32 SampleCount = 3;

    FMLP353 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP353::HiddenSize];
            float Output[FMLP353::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP353::OutSize; ++o)
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

