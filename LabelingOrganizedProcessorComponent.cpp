#include "LabelingOrganizedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingOrganizedProcessorComponent::ULabelingOrganizedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingOrganizedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign labels to groups
    const TArray<int32> GroupLabels = AssignLabelsToGroupsFlat(
        DemoAllGroupProbabilitiesFlat, DemoGroupRows, DemoNumLabels);

    for (int32 i = 0; i < GroupLabels.Num(); ++i)
    {
        UE_LOG(LogTemp, Log, TEXT("Group %d assigned label %d"), i, GroupLabels[i]);
    }

    // Train tiny neural model
    TrainGroupLabelingNeuralNetwork();
}

TArray<int32> ULabelingOrganizedProcessorComponent::AssignLabelsToGroupsFlat(const TArray<float>& AllGroupProbFlat,
                                                                             const TArray<int32>& GroupRows,
                                                                             int32 NumLabels)
{
    TArray<int32> Labels;
    if (GroupRows.Num() == 0 || NumLabels <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToGroupsFlat: invalid GroupRows/NumLabels."));
        return Labels;
    }

    // Validate total size
    int32 TotalRows = 0;
    for (int32 R : GroupRows) { if (R < 0) { UE_LOG(LogTemp, Error, TEXT("Negative row count.")); return Labels; } TotalRows += R; }
    const int32 Expected = TotalRows * NumLabels;
    if (AllGroupProbFlat.Num() != Expected)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToGroupsFlat: flat size %d != sum(Rows)*NumLabels %d."), AllGroupProbFlat.Num(), Expected);
        return Labels;
    }

    Labels.SetNum(GroupRows.Num());
    int32 Offset = 0; // index into AllGroupProbFlat
    for (int32 g = 0; g < GroupRows.Num(); ++g)
    {
        const int32 Rows = GroupRows[g];
        if (Rows == 0)
        {
            Labels[g] = 0;
            continue;
        }

        // Sum probabilities per label
        TArray<double> Sums;
        Sums.Init(0.0, NumLabels);
        for (int32 r = 0; r < Rows; ++r)
        {
            const float* RowPtr = AllGroupProbFlat.GetData() + Offset + r * NumLabels;
            for (int32 c = 0; c < NumLabels; ++c)
            {
                Sums[c] += static_cast<double>(RowPtr[c]);
            }
        }

        Labels[g] = ArgMax(Sums.GetData(), NumLabels);
        Offset += Rows * NumLabels;
    }

    return Labels;
}

int32 ULabelingOrganizedProcessorComponent::ArgMax(const double* Data, int32 Count)
{
    if (Count <= 0) return 0;
    int32 BestIdx = 0;
    double BestVal = Data[0];
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

ULabelingOrganizedProcessorComponent::FMLP352::FMLP352()
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

float ULabelingOrganizedProcessorComponent::FMLP352::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingOrganizedProcessorComponent::FMLP352::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingOrganizedProcessorComponent::FMLP352::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingOrganizedProcessorComponent::FMLP352::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingOrganizedProcessorComponent::TrainGroupLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: probability sums per group (toy data), Outputs: one-hot labels
    const float Inputs[3][3] = {
        { 2.0f, 1.0f, 0.5f },
        { 0.8f, 2.2f, 1.1f },
        { 1.5f, 0.9f, 1.8f }
    };
    const float Targets[3][2] = {
        { 1.f, 0.f },
        { 0.f, 1.f },
        { 1.f, 0.f }
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

