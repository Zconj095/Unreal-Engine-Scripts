#include "LabelingUnorganizedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingUnorganizedProcessorComponent::ULabelingUnorganizedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingUnorganizedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Assign labels with cosine similarity weighting
    const TArray<int32> Labels = AssignLabelsToUnorganizedDataFlatCosine(
        DemoDataFlat, DemoRows, DemoDims,
        DemoProbabilitiesFlat, DemoNumClasses);

    FString LStr;
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
        LStr += FString::FromInt(Labels[i]);
        if (i + 1 < Labels.Num()) LStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels for Unorganized Data: %s"), *LStr);

    // Train tiny neural model
    TrainUnorganizedLabelingNeuralNetwork();
}

TArray<int32> ULabelingUnorganizedProcessorComponent::AssignLabelsToUnorganizedDataFlatCosine(const TArray<float>& DataFlat, int32 Rows, int32 Dims,
                                                                                               const TArray<float>& ProbFlat, int32 NumClasses)
{
    TArray<int32> OutLabels;
    if (Rows <= 0 || Dims <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToUnorganizedDataFlatCosine: invalid dims."));
        return OutLabels;
    }
    if (DataFlat.Num() != Rows * Dims)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToUnorganizedDataFlatCosine: data size %d != Rows*Dims %d."), DataFlat.Num(), Rows * Dims);
        return OutLabels;
    }
    if (ProbFlat.Num() != Rows * NumClasses)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToUnorganizedDataFlatCosine: prob size %d != Rows*NumClasses %d."), ProbFlat.Num(), Rows * NumClasses);
        return OutLabels;
    }

    OutLabels.SetNum(Rows);

    // Precompute norms
    TArray<float> Norms;
    Norms.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        Norms[r] = Norm(DataFlat.GetData() + r * Dims, Dims);
        if (Norms[r] <= KINDA_SMALL_NUMBER)
        {
            Norms[r] = 1e-6f; // avoid div-by-zero
        }
    }

    for (int32 i = 0; i < Rows; ++i)
    {
        const float* Xi = DataFlat.GetData() + i * Dims;
        double ScoresMax = -DBL_MAX;
        int32 BestK = 0;

        // Accumulate scores for each class
        for (int32 k = 0; k < NumClasses; ++k)
        {
            double Sum = 0.0;
            for (int32 j = 0; j < Rows; ++j)
            {
                const float* Xj = DataFlat.GetData() + j * Dims;
                const float Cos = (Dot(Xi, Xj, Dims)) / (Norms[i] * Norms[j]);
                Sum += static_cast<double>(Cos) * static_cast<double>(ProbFlat[j * NumClasses + k]);
            }
            if (Sum > ScoresMax)
            {
                ScoresMax = Sum;
                BestK = k;
            }
        }
        OutLabels[i] = BestK;
    }

    return OutLabels;
}

float ULabelingUnorganizedProcessorComponent::Dot(const float* A, const float* B, int32 Dims)
{
    double s = 0.0;
    for (int32 d = 0; d < Dims; ++d)
    {
        s += static_cast<double>(A[d]) * static_cast<double>(B[d]);
    }
    return static_cast<float>(s);
}

float ULabelingUnorganizedProcessorComponent::Norm(const float* A, int32 Dims)
{
    double s = 0.0;
    for (int32 d = 0; d < Dims; ++d)
    {
        s += static_cast<double>(A[d]) * static_cast<double>(A[d]);
    }
    return static_cast<float>(FMath::Sqrt(s));
}

float ULabelingUnorganizedProcessorComponent::CosineSim(const float* A, const float* B, int32 Dims)
{
    const float nA = Norm(A, Dims);
    const float nB = Norm(B, Dims);
    if (nA <= KINDA_SMALL_NUMBER || nB <= KINDA_SMALL_NUMBER) return 0.0f;
    return Dot(A, B, Dims) / (nA * nB);
}

int32 ULabelingUnorganizedProcessorComponent::ArgMax(const double* Data, int32 Count)
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

ULabelingUnorganizedProcessorComponent::FMLP352::FMLP352()
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

float ULabelingUnorganizedProcessorComponent::FMLP352::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingUnorganizedProcessorComponent::FMLP352::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingUnorganizedProcessorComponent::FMLP352::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingUnorganizedProcessorComponent::FMLP352::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingUnorganizedProcessorComponent::TrainUnorganizedLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 3 features; Outputs: one-hot [C0, C1]
    const float Inputs[3][3] = {
        { 0.5f, 0.3f, 0.2f },
        { 0.7f, 0.6f, 0.5f },
        { 0.9f, 0.8f, 0.7f }
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

