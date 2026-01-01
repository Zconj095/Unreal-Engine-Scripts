#include "LabelingNoncategorizedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingNoncategorizedProcessorComponent::ULabelingNoncategorizedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingNoncategorizedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Perform clustering on demo data (to 2 clusters)
    const TArray<int32> Groups = PerformHierarchicalClusteringFlat(DemoDataFlat, DemoRows, DemoCols, 2);
    FString GroupStr;
    for (int32 i = 0; i < Groups.Num(); ++i)
    {
        GroupStr += FString::FromInt(Groups[i]);
        if (i + 1 < Groups.Num()) GroupStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Hierarchical Clustering Groups: %s"), *GroupStr);

    // Train tiny neural demo
    TrainNoncategorizedLabelingNeuralNetwork();
}

TArray<int32> ULabelingNoncategorizedProcessorComponent::PerformHierarchicalClusteringFlat(const TArray<float>& DataFlat, int32 Rows, int32 Cols, int32 TargetClusters)
{
    TArray<int32> Groups;
    if (Rows <= 0 || Cols <= 0 || TargetClusters <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("PerformHierarchicalClusteringFlat: invalid dims/target."));
        return Groups;
    }
    if (DataFlat.Num() != Rows * Cols)
    {
        UE_LOG(LogTemp, Error, TEXT("PerformHierarchicalClusteringFlat: Data size %d != Rows*Cols %d."), DataFlat.Num(), Rows * Cols);
        return Groups;
    }

    // Initialize groups 0..Rows-1
    Groups.SetNum(Rows);
    for (int32 i = 0; i < Rows; ++i) Groups[i] = i;

    // Precompute pairwise distances
    TArray<float> Dist; // Upper-tri or full; we will store full for simplicity (Rows x Rows)
    Dist.SetNumZeroed(Rows * Rows);
    for (int32 i = 0; i < Rows; ++i)
    {
        const float* Ai = DataFlat.GetData() + i * Cols;
        for (int32 j = i + 1; j < Rows; ++j)
        {
            const float* Aj = DataFlat.GetData() + j * Cols;
            const float d = EuclideanDistanceRows(Ai, Aj, Cols);
            Dist[i * Rows + j] = d;
            Dist[j * Rows + i] = d;
        }
    }

    auto CountDistinct = [&]() -> int32
    {
        TSet<int32> S;
        for (int32 g : Groups) S.Add(g);
        return S.Num();
    };

    // Agglomerate until target clusters reached
    while (CountDistinct() > TargetClusters)
    {
        float MinDist = TNumericLimits<float>::Max();
        int32 BestI = -1, BestJ = -1;

        for (int32 i = 0; i < Rows; ++i)
        {
            for (int32 j = i + 1; j < Rows; ++j)
            {
                if (Groups[i] != Groups[j])
                {
                    const float d = Dist[i * Rows + j];
                    if (d < MinDist)
                    {
                        MinDist = d;
                        BestI = i;
                        BestJ = j;
                    }
                }
            }
        }

        if (BestI < 0 || BestJ < 0)
        {
            break; // nothing to merge
        }

        const int32 ClusterA = Groups[BestI];
        const int32 ClusterB = Groups[BestJ];
        const int32 Merged = FMath::Min(ClusterA, ClusterB);
        const int32 ToReplace = FMath::Max(ClusterA, ClusterB);
        for (int32 i = 0; i < Rows; ++i)
        {
            if (Groups[i] == ToReplace)
            {
                Groups[i] = Merged;
            }
        }
    }

    return Groups;
}

float ULabelingNoncategorizedProcessorComponent::EuclideanDistanceRows(const float* A, const float* B, int32 Cols)
{
    double SumSq = 0.0;
    for (int32 c = 0; c < Cols; ++c)
    {
        const double d = static_cast<double>(A[c]) - static_cast<double>(B[c]);
        SumSq += d * d;
    }
    return static_cast<float>(FMath::Sqrt(SumSq));
}

// -------------------- Simple 3-5-2 MLP --------------------

ULabelingNoncategorizedProcessorComponent::FMLP352::FMLP352()
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

float ULabelingNoncategorizedProcessorComponent::FMLP352::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingNoncategorizedProcessorComponent::FMLP352::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingNoncategorizedProcessorComponent::FMLP352::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingNoncategorizedProcessorComponent::FMLP352::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingNoncategorizedProcessorComponent::TrainNoncategorizedLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs and one-hot cluster labels
    const float Inputs[4][3] = {
        { 1.0f, 0.5f, 0.2f },
        { 0.8f, 0.3f, 0.6f },
        { 0.4f, 0.9f, 0.7f },
        { 0.6f, 0.7f, 0.8f }
    };
    const float Targets[4][2] = {
        { 1.f, 0.f },
        { 0.f, 1.f },
        { 1.f, 0.f },
        { 0.f, 1.f }
    };
    const int32 SampleCount = 4;

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

