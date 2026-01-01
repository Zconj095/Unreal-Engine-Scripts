#include "LabelingMultifacetedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingMultifacetedProcessorComponent::ULabelingMultifacetedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingMultifacetedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign labels to facets
    const TArray<FFacetLabelRow> FacetLabels = AssignLabelsToFacetsFlat(
        DemoAllFacetProbabilitiesFlat, DemoRows, DemoNumFacets, DemoNumClasses);
    FString FacetStr;
    for (int32 r = 0; r < FacetLabels.Num(); ++r)
    {
        FacetStr += FString::Printf(TEXT("Data Point %d: ["), r);
        for (int32 f = 0; f < FacetLabels[r].FacetLabels.Num(); ++f)
        {
            FacetStr += FString::FromInt(FacetLabels[r].FacetLabels[f]);
            if (f + 1 < FacetLabels[r].FacetLabels.Num()) FacetStr += TEXT(", ");
        }
        FacetStr += TEXT("]\n");
    }
    UE_LOG(LogTemp, Log, TEXT("Facet Labels:\n%s"), *FacetStr);

    // Assign interdependent labels
    const TArray<int32> Interdependent = AssignInterdependentLabelsFlat(
        DemoAllFacetProbabilitiesFlat, DemoRows, DemoNumFacets, DemoNumClasses);
    FString InterStr;
    for (int32 i = 0; i < Interdependent.Num(); ++i)
    {
        InterStr += FString::FromInt(Interdependent[i]);
        if (i + 1 < Interdependent.Num()) InterStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Interdependent Labels: %s"), *InterStr);

    // Train tiny neural demo
    TrainMultifacetedLabelingNeuralNetwork();
}

TArray<FFacetLabelRow> ULabelingMultifacetedProcessorComponent::AssignLabelsToFacetsFlat(const TArray<float>& AllFacetProbFlat,
                                                                                          int32 Rows, int32 NumFacets, int32 NumClasses)
{
    TArray<FFacetLabelRow> Out;
    if (Rows <= 0 || NumFacets <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToFacetsFlat: invalid dims."));
        return Out;
    }
    const int32 Expected = Rows * NumClasses * NumFacets;
    if (AllFacetProbFlat.Num() != Expected)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToFacetsFlat: size %d != Rows*NumClasses*NumFacets %d."), AllFacetProbFlat.Num(), Expected);
        return Out;
    }

    Out.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        FFacetLabelRow Row;
        Row.FacetLabels.SetNum(NumFacets);
        for (int32 f = 0; f < NumFacets; ++f)
        {
            const float* RowPtr = AllFacetProbFlat.GetData() + (f * Rows + r) * NumClasses;
            Row.FacetLabels[f] = ArgMax(RowPtr, NumClasses);
        }
        Out[r] = MoveTemp(Row);
    }
    return Out;
}

TArray<int32> ULabelingMultifacetedProcessorComponent::AssignInterdependentLabelsFlat(const TArray<float>& AllFacetProbFlat,
                                                                                      int32 Rows, int32 NumFacets, int32 NumClasses)
{
    TArray<int32> Out;
    if (Rows <= 0 || NumFacets <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignInterdependentLabelsFlat: invalid dims."));
        return Out;
    }
    const int32 Expected = Rows * NumClasses * NumFacets;
    if (AllFacetProbFlat.Num() != Expected)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignInterdependentLabelsFlat: size %d != Rows*NumClasses*NumFacets %d."), AllFacetProbFlat.Num(), Expected);
        return Out;
    }

    Out.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        // Initialize products to 1.0 (not 0.0) so multiplication accumulates correctly
        TArray<double> ProductScores;
        ProductScores.Init(1.0, NumClasses);

        for (int32 f = 0; f < NumFacets; ++f)
        {
            const float* RowPtr = AllFacetProbFlat.GetData() + (f * Rows + r) * NumClasses;
            for (int32 k = 0; k < NumClasses; ++k)
            {
                ProductScores[k] *= static_cast<double>(RowPtr[k]);
            }
        }

        // Find argmax
        int32 BestIdx = 0;
        double BestVal = ProductScores[0];
        for (int32 k = 1; k < NumClasses; ++k)
        {
            if (ProductScores[k] > BestVal)
            {
                BestVal = ProductScores[k];
                BestIdx = k;
            }
        }
        Out[r] = BestIdx;
    }
    return Out;
}

int32 ULabelingMultifacetedProcessorComponent::ArgMax(const float* Data, int32 Count)
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

// -------------------- Simple 4-8-3 MLP --------------------

ULabelingMultifacetedProcessorComponent::FMLP483::FMLP483()
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

float ULabelingMultifacetedProcessorComponent::FMLP483::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingMultifacetedProcessorComponent::FMLP483::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingMultifacetedProcessorComponent::FMLP483::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingMultifacetedProcessorComponent::FMLP483::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingMultifacetedProcessorComponent::TrainMultifacetedLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 4 features; Outputs: one-hot [F1, F2, F3]
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

    FMLP483 Net;

    for (int32 Epoch = 0; Epoch < MaxEpochs; ++Epoch)
    {
        double EpochError = 0.0;
        for (int32 s = 0; s < SampleCount; ++s)
        {
            Net.TrainSGD(Inputs[s], Targets[s], LearningRate);

            float Hidden[FMLP483::HiddenSize];
            float Output[FMLP483::OutSize];
            Net.Forward(Inputs[s], Hidden, Output);
            for (int32 o = 0; o < FMLP483::OutSize; ++o)
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

