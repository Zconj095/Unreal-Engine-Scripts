#include "LabelingProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingProcessorComponent::ULabelingProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Probability-based labeling demo
    const TArray<int32> ByProb = AssignLabelsByProbabilityFlat(DemoProbabilitiesFlat, DemoProbRows, DemoProbCols);
    FString ProbStr;
    for (int32 i = 0; i < ByProb.Num(); ++i)
    {
        ProbStr += FString::FromInt(ByProb[i]);
        if (i + 1 < ByProb.Num()) ProbStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels by Probability: %s"), *ProbStr);

    // Distance-based labeling demo
    const TArray<int32> ByDist = AssignLabelsByDistanceFlat(DemoItemsFlat, DemoItemRows, DemoItemDims, DemoCentroidsFlat, DemoCentroidRows);
    FString DistStr;
    for (int32 i = 0; i < ByDist.Num(); ++i)
    {
        DistStr += FString::FromInt(ByDist[i]);
        if (i + 1 < ByDist.Num()) DistStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels by Distance: %s"), *DistStr);

    // Tiny neural demo
    TrainLabelingNeuralNetwork();
}

TArray<int32> ULabelingProcessorComponent::AssignLabelsByProbabilityFlat(const TArray<float>& ProbFlat, int32 Rows, int32 Cols)
{
    TArray<int32> Labels;
    if (Rows <= 0 || Cols <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsByProbabilityFlat: invalid dims."));
        return Labels;
    }
    if (ProbFlat.Num() != Rows * Cols)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsByProbabilityFlat: size %d != Rows*Cols %d."), ProbFlat.Num(), Rows * Cols);
        return Labels;
    }

    Labels.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* RowPtr = ProbFlat.GetData() + r * Cols;
        Labels[r] = ArgMax(RowPtr, Cols);
    }
    return Labels;
}

TArray<int32> ULabelingProcessorComponent::AssignLabelsByDistanceFlat(const TArray<float>& ItemsFlat, int32 ItemRows, int32 Dims,
                                                                       const TArray<float>& CentroidsFlat, int32 CentroidRows)
{
    TArray<int32> Labels;
    if (ItemRows <= 0 || Dims <= 0 || CentroidRows <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsByDistanceFlat: invalid dims."));
        return Labels;
    }
    if (ItemsFlat.Num() != ItemRows * Dims)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsByDistanceFlat: items size %d != Rows*Dims %d."), ItemsFlat.Num(), ItemRows * Dims);
        return Labels;
    }
    if (CentroidsFlat.Num() != CentroidRows * Dims)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsByDistanceFlat: centroids size %d != Rows*Dims %d."), CentroidsFlat.Num(), CentroidRows * Dims);
        return Labels;
    }

    Labels.SetNum(ItemRows);
    for (int32 r = 0; r < ItemRows; ++r)
    {
        const float* X = ItemsFlat.GetData() + r * Dims;
        int32 BestIdx = 0;
        float BestDist = TNumericLimits<float>::Max();
        for (int32 k = 0; k < CentroidRows; ++k)
        {
            const float* Ck = CentroidsFlat.GetData() + k * Dims;
            const float d = EuclideanDistanceRows(X, Ck, Dims);
            if (d < BestDist)
            {
                BestDist = d;
                BestIdx = k;
            }
        }
        Labels[r] = BestIdx;
    }
    return Labels;
}

int32 ULabelingProcessorComponent::ArgMax(const float* Data, int32 Count)
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

float ULabelingProcessorComponent::EuclideanDistanceRows(const float* A, const float* B, int32 Dims)
{
    double SumSq = 0.0;
    for (int32 d = 0; d < Dims; ++d)
    {
        const double diff = static_cast<double>(A[d]) - static_cast<double>(B[d]);
        SumSq += diff * diff;
    }
    return static_cast<float>(FMath::Sqrt(SumSq));
}

// -------------------- Simple 3-5-2 MLP --------------------

ULabelingProcessorComponent::FMLP352::FMLP352()
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

float ULabelingProcessorComponent::FMLP352::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingProcessorComponent::FMLP352::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingProcessorComponent::FMLP352::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingProcessorComponent::FMLP352::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingProcessorComponent::TrainLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 3 features; Outputs: one-hot [L0, L1]
    const float Inputs[3][3] = {
        { 1.0f, 0.5f, 0.2f },
        { 0.8f, 0.3f, 0.6f },
        { 0.2f, 0.9f, 0.4f }
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

