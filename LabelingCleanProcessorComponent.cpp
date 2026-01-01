#include "LabelingCleanProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingCleanProcessorComponent::ULabelingCleanProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingCleanProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Demo: argmax labeling on flat probabilities
    const TArray<int32> Labels = AssignLabelsToCleanDataFlat(DemoProbabilitiesFlat, DemoRows, DemoCols);
    FString LabelsStr;
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
        LabelsStr += FString::FromInt(Labels[i]);
        if (i + 1 < Labels.Num()) LabelsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels for Clean Data: %s"), *LabelsStr);

    // Demo: tiny neural classifier
    TrainCleanDataLabelingNeuralNetwork();
}

TArray<int32> ULabelingCleanProcessorComponent::AssignLabelsToCleanData(const TArray<TArray<float>>& Probabilities)
{
    TArray<int32> Labels;
    Labels.SetNum(Probabilities.Num());
    for (int32 r = 0; r < Probabilities.Num(); ++r)
    {
        const TArray<float>& Row = Probabilities[r];
        if (Row.Num() == 0)
        {
            Labels[r] = 0;
            continue;
        }
        Labels[r] = ArgMax(Row.GetData(), Row.Num());
    }
    return Labels;
}

TArray<int32> ULabelingCleanProcessorComponent::AssignLabelsToCleanDataFlat(const TArray<float>& FlatRowMajor, int32 Rows, int32 Cols)
{
    TArray<int32> Labels;
    if (Rows <= 0 || Cols <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToCleanDataFlat: Rows and Cols must be > 0."));
        return Labels;
    }
    if (FlatRowMajor.Num() != Rows * Cols)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToCleanDataFlat: Flat array size %d != Rows*Cols %d."), FlatRowMajor.Num(), Rows * Cols);
        return Labels;
    }

    Labels.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* RowPtr = FlatRowMajor.GetData() + r * Cols;
        Labels[r] = ArgMax(RowPtr, Cols);
    }
    return Labels;
}

int32 ULabelingCleanProcessorComponent::ArgMax(const float* Data, int32 Count)
{
    if (Count <= 0)
    {
        return 0;
    }
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

ULabelingCleanProcessorComponent::FMLP3C::FMLP3C()
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

float ULabelingCleanProcessorComponent::FMLP3C::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingCleanProcessorComponent::FMLP3C::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingCleanProcessorComponent::FMLP3C::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingCleanProcessorComponent::FMLP3C::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingCleanProcessorComponent::TrainCleanDataLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: features of cleaned data; Outputs: one-hot labels
    const float Inputs[3][3] = {
        { 1.0f, 0.5f, 0.2f },
        { 0.8f, 0.3f, 0.6f },
        { 0.2f, 0.9f, 0.4f }
    };
    const float Targets[3][2] = {
        { 1.f, 0.f }, // Label 0
        { 0.f, 1.f }, // Label 1
        { 1.f, 0.f }  // Label 0
    };
    const int32 SampleCount = 3;

    FMLP3C Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP3C::HiddenSize];
            float Output[FMLP3C::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP3C::OutSize; ++o)
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

