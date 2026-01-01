#include "LabelingNonclassifiedProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingNonclassifiedProcessorComponent::ULabelingNonclassifiedProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingNonclassifiedProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Run K-Means on demo data
    const TArray<int32> Labels = AssignLabelsToClustersFlat(DemoDataFlat, DemoRows, DemoCols, DemoNumClusters);
    FString LabelStr;
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
        LabelStr += FString::FromInt(Labels[i]);
        if (i + 1 < Labels.Num()) LabelStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Cluster Labels: %s"), *LabelStr);

    // Train tiny neural demo
    TrainNonclassifiedLabelingNeuralNetwork();
}

TArray<int32> ULabelingNonclassifiedProcessorComponent::AssignLabelsToClustersFlat(const TArray<float>& DataFlat, int32 Rows, int32 Cols,
                                                                                     int32 NumClusters, int32 MaxIters)
{
    TArray<int32> Labels;
    if (Rows <= 0 || Cols <= 0 || NumClusters <= 0 || NumClusters > Rows)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToClustersFlat: invalid dims/k."));
        return Labels;
    }
    if (DataFlat.Num() != Rows * Cols)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToClustersFlat: data size %d != Rows*Cols %d."), DataFlat.Num(), Rows * Cols);
        return Labels;
    }

    Labels.SetNum(Rows);

    // Initialize centroids with first K rows (deterministic)
    TArray<float> Centroids;
    Centroids.SetNumZeroed(NumClusters * Cols);
    for (int32 k = 0; k < NumClusters; ++k)
    {
        const float* RowPtr = DataFlat.GetData() + k * Cols;
        float* Ck = Centroids.GetData() + k * Cols;
        for (int32 c = 0; c < Cols; ++c)
        {
            Ck[c] = RowPtr[c];
        }
    }

    TArray<int32> PrevLabels;
    PrevLabels.Init(-1, Rows);

    for (int32 iter = 0; iter < MaxIters; ++iter)
    {
        // Assign step
        for (int32 r = 0; r < Rows; ++r)
        {
            const float* X = DataFlat.GetData() + r * Cols;
            int32 BestK = 0;
            float BestD = TNumericLimits<float>::Max();
            for (int32 k = 0; k < NumClusters; ++k)
            {
                const float* Ck = Centroids.GetData() + k * Cols;
                const float d = EuclideanDistanceRows(X, Ck, Cols);
                if (d < BestD)
                {
                    BestD = d;
                    BestK = k;
                }
            }
            Labels[r] = BestK;
        }

        // Check convergence
        bool bSame = true;
        for (int32 r = 0; r < Rows; ++r)
        {
            if (Labels[r] != PrevLabels[r])
            {
                bSame = false;
                break;
            }
        }
        if (bSame && iter > 0)
        {
            break;
        }
        PrevLabels = Labels;

        // Update step: compute means
        TArray<double> Sum;
        Sum.SetNumZeroed(NumClusters * Cols);
        TArray<int32> Count;
        Count.Init(0, NumClusters);

        for (int32 r = 0; r < Rows; ++r)
        {
            const int32 k = Labels[r];
            const float* X = DataFlat.GetData() + r * Cols;
            double* S = Sum.GetData() + k * Cols;
            for (int32 c = 0; c < Cols; ++c)
            {
                S[c] += static_cast<double>(X[c]);
            }
            Count[k] += 1;
        }

        for (int32 k = 0; k < NumClusters; ++k)
        {
            float* Ck = Centroids.GetData() + k * Cols;
            if (Count[k] > 0)
            {
                const double Inv = 1.0 / static_cast<double>(Count[k]);
                const double* S = Sum.GetData() + k * Cols;
                for (int32 c = 0; c < Cols; ++c)
                {
                    Ck[c] = static_cast<float>(S[c] * Inv);
                }
            }
            else
            {
                // Reinitialize empty cluster to a random point to avoid dead centroid
                const int32 r = FMath::RandRange(0, Rows - 1);
                const float* X = DataFlat.GetData() + r * Cols;
                for (int32 c = 0; c < Cols; ++c)
                {
                    Ck[c] = X[c];
                }
            }
        }
    }

    return Labels;
}

float ULabelingNonclassifiedProcessorComponent::EuclideanDistanceRows(const float* A, const float* B, int32 Cols)
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

ULabelingNonclassifiedProcessorComponent::FMLP352::FMLP352()
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

float ULabelingNonclassifiedProcessorComponent::FMLP352::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingNonclassifiedProcessorComponent::FMLP352::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingNonclassifiedProcessorComponent::FMLP352::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingNonclassifiedProcessorComponent::FMLP352::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingNonclassifiedProcessorComponent::TrainNonclassifiedLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
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

