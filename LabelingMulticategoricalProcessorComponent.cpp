#include "LabelingMulticategoricalProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingMulticategoricalProcessorComponent::ULabelingMulticategoricalProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingMulticategoricalProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign multicategorical labels from demo probabilities
    const TArray<FMultiCategoryLabels> Result = AssignMulticategoricalLabelsFlat(
        DemoProbabilitiesFlat, DemoRows, DemoNumCategories, DemoThreshold);

    for (int32 i = 0; i < Result.Num(); ++i)
    {
        FString Cats;
        for (int32 j = 0; j < Result[i].Categories.Num(); ++j)
        {
            Cats += FString::FromInt(Result[i].Categories[j]);
            if (j + 1 < Result[i].Categories.Num()) Cats += TEXT(", ");
        }
        UE_LOG(LogTemp, Log, TEXT("Data Point %d: Categories - %s"), i, *Cats);
    }

    // Train tiny neural example
    TrainMulticategoricalLabelingNeuralNetwork();
}

TArray<TArray<int32>> ULabelingMulticategoricalProcessorComponent::AssignMulticategoricalLabels(const TArray<float>& ProbFlat, int32 Rows, int32 NumCategories, float Threshold)
{
    TArray<TArray<int32>> Out;
    if (Rows <= 0 || NumCategories <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignMulticategoricalLabels: invalid dims."));
        return Out;
    }
    if (ProbFlat.Num() != Rows * NumCategories)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignMulticategoricalLabels: size %d != Rows*NumCategories %d."), ProbFlat.Num(), Rows * NumCategories);
        return Out;
    }

    Out.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        TArray<int32>& Cats = Out[r];
        for (int32 c = 0; c < NumCategories; ++c)
        {
            const float P = ProbFlat[r * NumCategories + c];
            if (P > Threshold)
            {
                Cats.Add(c);
            }
        }
    }
    return Out;
}

TArray<FMultiCategoryLabels> ULabelingMulticategoricalProcessorComponent::AssignMulticategoricalLabelsFlat(const TArray<float>& ProbFlat, int32 Rows, int32 NumCategories, float Threshold)
{
    TArray<FMultiCategoryLabels> Out;
    if (Rows <= 0 || NumCategories <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignMulticategoricalLabelsFlat: invalid dims."));
        return Out;
    }
    if (ProbFlat.Num() != Rows * NumCategories)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignMulticategoricalLabelsFlat: size %d != Rows*NumCategories %d."), ProbFlat.Num(), Rows * NumCategories);
        return Out;
    }

    Out.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        FMultiCategoryLabels RowLabels;
        for (int32 c = 0; c < NumCategories; ++c)
        {
            const float P = ProbFlat[r * NumCategories + c];
            if (P > Threshold)
            {
                RowLabels.Categories.Add(c);
            }
        }
        Out[r] = MoveTemp(RowLabels);
    }
    return Out;
}

// -------------------- Simple 4-6-3 MLP --------------------

ULabelingMulticategoricalProcessorComponent::FMLP463::FMLP463()
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

float ULabelingMulticategoricalProcessorComponent::FMLP463::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingMulticategoricalProcessorComponent::FMLP463::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingMulticategoricalProcessorComponent::FMLP463::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingMulticategoricalProcessorComponent::FMLP463::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingMulticategoricalProcessorComponent::TrainMulticategoricalLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 4 features; Outputs: 3 probabilities (multi-label style here uses MSE on sigmoid outputs)
    const float Inputs[3][4] = {
        { 0.5f, 0.3f, 0.2f, 0.1f },
        { 0.7f, 0.6f, 0.5f, 0.4f },
        { 0.9f, 0.8f, 0.7f, 0.6f }
    };
    const float Targets[3][3] = {
        { 0.9f, 0.2f, 0.1f },
        { 0.4f, 0.6f, 0.2f },
        { 0.7f, 0.3f, 0.8f }
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

