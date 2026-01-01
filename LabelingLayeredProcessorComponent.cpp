#include "LabelingLayeredProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingLayeredProcessorComponent::ULabelingLayeredProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingLayeredProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign layered labels (strings)
    const TArray<FString> Layered = AssignLayeredLabelsFlat(
        DemoAllLayerProbabilitiesFlat, DemoRows, DemoNumLayers, DemoNumClasses);
    FString OutStr;
    for (int32 i = 0; i < Layered.Num(); ++i)
    {
        OutStr += Layered[i];
        if (i + 1 < Layered.Num()) OutStr += TEXT("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("Layered Labels:\n%s"), *OutStr);

    // Assign weighted layered labels
    const TArray<int32> Weighted = AssignWeightedLayeredLabelsFlat(
        DemoAllLayerProbabilitiesFlat, DemoRows, DemoNumLayers, DemoNumClasses, DemoLayerWeights);
    FString WStr;
    for (int32 i = 0; i < Weighted.Num(); ++i)
    {
        WStr += FString::FromInt(Weighted[i]);
        if (i + 1 < Weighted.Num()) WStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Weighted Layered Labels: %s"), *WStr);

    // Train tiny neural model
    TrainLayeredLabelingNeuralNetwork();
}

TArray<FString> ULabelingLayeredProcessorComponent::AssignLayeredLabelsFlat(const TArray<float>& AllLayerProbFlat,
                                                                            int32 Rows, int32 NumLayers, int32 NumClasses)
{
    TArray<FString> Out;
    if (Rows <= 0 || NumLayers <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLayeredLabelsFlat: invalid dims."));
        return Out;
    }
    const int32 Expected = Rows * NumClasses * NumLayers;
    if (AllLayerProbFlat.Num() != Expected)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLayeredLabelsFlat: size %d != Rows*NumClasses*NumLayers %d."), AllLayerProbFlat.Num(), Expected);
        return Out;
    }

    Out.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        FString Label = FString::Printf(TEXT("Data Point %d: {"), r);
        for (int32 l = 0; l < NumLayers; ++l)
        {
            const float* RowPtr = AllLayerProbFlat.GetData() + (l * Rows + r) * NumClasses;
            const int32 Best = ArgMax(RowPtr, NumClasses);
            Label += FString::Printf(TEXT("Layer-%d: %d"), l, Best);
            if (l + 1 < NumLayers) Label += TEXT(", ");
        }
        Label += TEXT("}");
        Out[r] = Label;
    }
    return Out;
}

TArray<int32> ULabelingLayeredProcessorComponent::AssignWeightedLayeredLabelsFlat(const TArray<float>& AllLayerProbFlat,
                                                                                  int32 Rows, int32 NumLayers, int32 NumClasses,
                                                                                  const TArray<float>& LayerWeights)
{
    TArray<int32> Out;
    if (Rows <= 0 || NumLayers <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignWeightedLayeredLabelsFlat: invalid dims."));
        return Out;
    }
    const int32 Expected = Rows * NumClasses * NumLayers;
    if (AllLayerProbFlat.Num() != Expected)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignWeightedLayeredLabelsFlat: size %d != Rows*NumClasses*NumLayers %d."), AllLayerProbFlat.Num(), Expected);
        return Out;
    }
    if (LayerWeights.Num() != NumLayers)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignWeightedLayeredLabelsFlat: LayerWeights size %d != NumLayers %d."), LayerWeights.Num(), NumLayers);
        return Out;
    }

    Out.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        TArray<float> Scores;
        Scores.Init(0.f, NumClasses);
        for (int32 l = 0; l < NumLayers; ++l)
        {
            const float* RowPtr = AllLayerProbFlat.GetData() + (l * Rows + r) * NumClasses;
            const float W = LayerWeights[l];
            for (int32 c = 0; c < NumClasses; ++c)
            {
                Scores[c] += W * RowPtr[c];
            }
        }
        Out[r] = ArgMax(Scores.GetData(), NumClasses);
    }
    return Out;
}

int32 ULabelingLayeredProcessorComponent::ArgMax(const float* Data, int32 Count)
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

ULabelingLayeredProcessorComponent::FMLP463::FMLP463()
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

float ULabelingLayeredProcessorComponent::FMLP463::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingLayeredProcessorComponent::FMLP463::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingLayeredProcessorComponent::FMLP463::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingLayeredProcessorComponent::FMLP463::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingLayeredProcessorComponent::TrainLayeredLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 4 features; Outputs: one-hot [L0, L1, L2]
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

