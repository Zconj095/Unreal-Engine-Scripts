#include "LabelingContextualProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingContextualProcessorComponent::ULabelingContextualProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingContextualProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign labels with context
    const TArray<int32> LabelsWithContext = AssignLabelsWithContextFlat(
        DemoDataFlat, DemoRows, DemoDataCols,
        DemoContextFlat, DemoContextCols,
        DemoProbabilitiesFlat, DemoNumClasses);

    FString LabelsStr;
    for (int32 i = 0; i < LabelsWithContext.Num(); ++i)
    {
        LabelsStr += FString::FromInt(LabelsWithContext[i]);
        if (i + 1 < LabelsWithContext.Num()) LabelsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels with Context: %s"), *LabelsStr);

    // Dynamic context labels
    const TArray<int32> DynLabels = AssignDynamicContextLabels(DemoBaseLabels, DemoContextualAdjustments);
    FString DynStr;
    for (int32 i = 0; i < DynLabels.Num(); ++i)
    {
        DynStr += FString::FromInt(DynLabels[i]);
        if (i + 1 < DynLabels.Num()) DynStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Dynamic Context Labels: %s"), *DynStr);

    // Train demo neural model
    TrainContextualLabelingNeuralNetwork();
}

TArray<int32> ULabelingContextualProcessorComponent::AssignLabelsWithContextFlat(const TArray<float>& DataFlat, int32 Rows, int32 DataCols,
                                                                                  const TArray<float>& ContextFlat, int32 ContextCols,
                                                                                  const TArray<float>& ProbFlat, int32 NumClasses)
{
    TArray<int32> Labels;
    if (Rows <= 0 || DataCols <= 0 || ContextCols <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsWithContextFlat: invalid dimensions."));
        return Labels;
    }
    if (DataCols != ContextCols)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsWithContextFlat: DataCols %d != ContextCols %d."), DataCols, ContextCols);
        return Labels;
    }
    if (DataFlat.Num() != Rows * DataCols)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsWithContextFlat: Data size %d != Rows*DataCols %d."), DataFlat.Num(), Rows * DataCols);
        return Labels;
    }
    if (ContextFlat.Num() != Rows * ContextCols)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsWithContextFlat: Context size %d != Rows*ContextCols %d."), ContextFlat.Num(), Rows * ContextCols);
        return Labels;
    }
    if (ProbFlat.Num() != Rows * NumClasses)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsWithContextFlat: Prob size %d != Rows*NumClasses %d."), ProbFlat.Num(), Rows * NumClasses);
        return Labels;
    }

    // Combine data + context (not used further in argmax, but computed to mirror C# logic)
    // You could plug this into a downstream probability model if needed.
    TArray<float> Combined;
    Combined.SetNumUninitialized(Rows * DataCols);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* X = DataFlat.GetData() + r * DataCols;
        const float* C = ContextFlat.GetData() + r * ContextCols;
        float* Z = Combined.GetData() + r * DataCols;
        for (int32 c = 0; c < DataCols; ++c)
        {
            Z[c] = X[c] + C[c];
        }
    }

    Labels.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* P = ProbFlat.GetData() + r * NumClasses;
        Labels[r] = ArgMax(P, NumClasses);
    }
    return Labels;
}

TArray<int32> ULabelingContextualProcessorComponent::AssignDynamicContextLabels(const TArray<int32>& BaseLabels, const TArray<int32>& ContextualAdjustments)
{
    TArray<int32> Out;
    if (BaseLabels.Num() != ContextualAdjustments.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("AssignDynamicContextLabels: size mismatch %d vs %d."), BaseLabels.Num(), ContextualAdjustments.Num());
        return Out;
    }
    Out.SetNum(BaseLabels.Num());
    for (int32 i = 0; i < BaseLabels.Num(); ++i)
    {
        Out[i] = BaseLabels[i] + ContextualAdjustments[i];
    }
    return Out;
}

int32 ULabelingContextualProcessorComponent::ArgMax(const float* Data, int32 Count)
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

// -------------------- Simple 4-6-3 MLP --------------------

ULabelingContextualProcessorComponent::FMLP463::FMLP463()
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

float ULabelingContextualProcessorComponent::FMLP463::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingContextualProcessorComponent::FMLP463::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingContextualProcessorComponent::FMLP463::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingContextualProcessorComponent::FMLP463::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingContextualProcessorComponent::TrainContextualLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: Combined data+context (4 features); Outputs: one-hot [C0, C1, C2]
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

