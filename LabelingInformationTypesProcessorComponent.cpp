#include "LabelingInformationTypesProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingInformationTypesProcessorComponent::ULabelingInformationTypesProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingInformationTypesProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign type labels via argmax
    const TArray<int32> TypeLabels = AssignTypeLabelsFlat(DemoProbabilitiesFlat, DemoRows, DemoNumTypes);
    FString LabelsStr;
    for (int32 i = 0; i < TypeLabels.Num(); ++i)
    {
        LabelsStr += FString::FromInt(TypeLabels[i]);
        if (i + 1 < TypeLabels.Num()) LabelsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Type Labels: %s"), *LabelsStr);

    // Train tiny neural model
    TrainTypeLabelingNeuralNetwork();
}

TArray<int32> ULabelingInformationTypesProcessorComponent::AssignTypeLabelsFlat(const TArray<float>& ProbFlat, int32 Rows, int32 NumTypes)
{
    TArray<int32> Labels;
    if (Rows <= 0 || NumTypes <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignTypeLabelsFlat: invalid dims."));
        return Labels;
    }
    if (ProbFlat.Num() != Rows * NumTypes)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignTypeLabelsFlat: size %d != Rows*NumTypes %d."), ProbFlat.Num(), Rows * NumTypes);
        return Labels;
    }

    Labels.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* RowPtr = ProbFlat.GetData() + r * NumTypes;
        Labels[r] = ArgMax(RowPtr, NumTypes);
    }
    return Labels;
}

int32 ULabelingInformationTypesProcessorComponent::ArgMax(const float* Data, int32 Count)
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

// -------------------- Simple 3-5-2 MLP --------------------

ULabelingInformationTypesProcessorComponent::FMLP352::FMLP352()
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

float ULabelingInformationTypesProcessorComponent::FMLP352::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingInformationTypesProcessorComponent::FMLP352::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingInformationTypesProcessorComponent::FMLP352::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingInformationTypesProcessorComponent::FMLP352::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingInformationTypesProcessorComponent::TrainTypeLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 3 features; Outputs: one-hot [T0, T1]
    const float Inputs[3][3] = {
        { 0.5f, 0.3f, 0.2f },
        { 0.7f, 0.6f, 0.5f },
        { 0.9f, 0.8f, 0.7f }
    };
    const float Targets[3][2] = {
        { 1.f, 0.f }, // Type 0
        { 0.f, 1.f }, // Type 1
        { 1.f, 0.f }  // Type 0
    };
    const int32 SampleCount = 3;

    FMLP352 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP352::HiddenSize];
            float Output[FMLP352::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP352::OutSize; ++o)
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

