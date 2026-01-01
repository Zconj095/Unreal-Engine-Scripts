#include "LabelingDenseProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

ULabelingDenseProcessorComponent::ULabelingDenseProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void ULabelingDenseProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // PCA reduction
    const TArray<float> Reduced = ApplyDimensionalityReductionFlatPCA(DemoDataFlat, DemoRows, DemoCols, DemoComponents);
    // Log reduced data
    FString RedStr;
    for (int32 r = 0; r < DemoRows; ++r)
    {
        RedStr += TEXT("[");
        for (int32 c = 0; c < DemoComponents; ++c)
        {
            const int32 idx = r * DemoComponents + c;
            RedStr += FString::SanitizeFloat(Reduced.IsValidIndex(idx) ? Reduced[idx] : 0.f);
            if (c + 1 < DemoComponents) RedStr += TEXT(", ");
        }
        RedStr += TEXT("] ");
    }
    UE_LOG(LogTemp, Log, TEXT("Reduced Data: %s"), *RedStr);

    // Assign labels to reduced data via probabilities
    const TArray<int32> Labels = AssignLabelsToReducedDataFlat(DemoProbabilitiesFlat, DemoRows, DemoNumClasses);
    FString LabelsStr;
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
        LabelsStr += FString::FromInt(Labels[i]);
        if (i + 1 < Labels.Num()) LabelsStr += TEXT(", ");
    }
    UE_LOG(LogTemp, Log, TEXT("Labels for Reduced Data: %s"), *LabelsStr);

    // Demo neural training (independent toy example)
    TrainReducedDataLabelingNeuralNetwork();
}

TArray<float> ULabelingDenseProcessorComponent::ApplyDimensionalityReductionFlatPCA(const TArray<float>& DataFlat, int32 Rows, int32 Cols, int32 Components)
{
    TArray<float> Reduced;
    if (Rows <= 0 || Cols <= 0 || Components <= 0 || Components > Cols)
    {
        UE_LOG(LogTemp, Error, TEXT("PCA: invalid dimensions Rows=%d Cols=%d Components=%d"), Rows, Cols, Components);
        return Reduced;
    }
    if (DataFlat.Num() != Rows * Cols)
    {
        UE_LOG(LogTemp, Error, TEXT("PCA: data size %d != Rows*Cols %d"), DataFlat.Num(), Rows * Cols);
        return Reduced;
    }

    // Copy and center columns
    TArray<float> Centered = DataFlat;
    TArray<float> Means;
    CenterColumns(Centered, Rows, Cols, Means);

    // Covariance (Cols x Cols)
    TArray<float> Cov = ComputeCovariance(Centered, Rows, Cols);
    const int32 N = Cols;

    // Power iteration to extract top-k eigenvectors with deflation
    TArray<float> W; // Cols x Components, column-major in blocks appended row-major by columns
    W.SetNumZeroed(N * Components);
    for (int32 k = 0; k < Components; ++k)
    {
        float Lambda = 0.f;
        TArray<float> V;
        PowerIteration(Cov, N, 200, 1e-6f, Lambda, V);

        // Store eigenvector as column k
        for (int32 i = 0; i < N; ++i)
        {
            W[k * N + i] = V.IsValidIndex(i) ? V[i] : 0.f; // store column-wise contiguous
        }

        // Deflate
        Deflate(Cov, N, Lambda, V);
    }

    // Project: Reduced = Centered (Rows x N) * W (N x Components)
    Reduced.SetNumZeroed(Rows * Components);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* X = Centered.GetData() + r * N;
        for (int32 c = 0; c < Components; ++c)
        {
            const float* Pc = W.GetData() + c * N; // column c
            float val = 0.f;
            for (int32 i = 0; i < N; ++i)
            {
                val += X[i] * Pc[i];
            }
            Reduced[r * Components + c] = val;
        }
    }

    return Reduced;
}

TArray<int32> ULabelingDenseProcessorComponent::AssignLabelsToReducedDataFlat(const TArray<float>& ProbFlat, int32 Rows, int32 NumClasses)
{
    TArray<int32> Labels;
    if (Rows <= 0 || NumClasses <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToReducedDataFlat: invalid dims."));
        return Labels;
    }
    if (ProbFlat.Num() != Rows * NumClasses)
    {
        UE_LOG(LogTemp, Error, TEXT("AssignLabelsToReducedDataFlat: size %d != Rows*NumClasses %d."), ProbFlat.Num(), Rows * NumClasses);
        return Labels;
    }

    Labels.SetNum(Rows);
    for (int32 r = 0; r < Rows; ++r)
    {
        const float* RowPtr = ProbFlat.GetData() + r * NumClasses;
        Labels[r] = ArgMax(RowPtr, NumClasses);
    }
    return Labels;
}

int32 ULabelingDenseProcessorComponent::ArgMax(const float* Data, int32 Count)
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

// ---------- PCA helpers ----------

void ULabelingDenseProcessorComponent::CenterColumns(TArray<float>& DataFlat, int32 Rows, int32 Cols, TArray<float>& OutMeans)
{
    OutMeans.SetNumZeroed(Cols);
    // Compute means per column
    for (int32 c = 0; c < Cols; ++c)
    {
        double Sum = 0.0;
        for (int32 r = 0; r < Rows; ++r)
        {
            Sum += static_cast<double>(DataFlat[r * Cols + c]);
        }
        OutMeans[c] = static_cast<float>(Sum / static_cast<double>(Rows));
    }
    // Subtract means
    for (int32 r = 0; r < Rows; ++r)
    {
        for (int32 c = 0; c < Cols; ++c)
        {
            DataFlat[r * Cols + c] -= OutMeans[c];
        }
    }
}

TArray<float> ULabelingDenseProcessorComponent::ComputeCovariance(const TArray<float>& CenteredData, int32 Rows, int32 Cols)
{
    TArray<float> Cov;
    Cov.SetNumZeroed(Cols * Cols);
    const double Scale = (Rows > 1) ? 1.0 / static_cast<double>(Rows - 1) : 1.0;
    for (int32 i = 0; i < Cols; ++i)
    {
        for (int32 j = i; j < Cols; ++j)
        {
            double Sum = 0.0;
            for (int32 r = 0; r < Rows; ++r)
            {
                Sum += static_cast<double>(CenteredData[r * Cols + i]) * static_cast<double>(CenteredData[r * Cols + j]);
            }
            const float Val = static_cast<float>(Sum * Scale);
            Cov[i * Cols + j] = Val;
            Cov[j * Cols + i] = Val;
        }
    }
    return Cov;
}

float ULabelingDenseProcessorComponent::VectorNorm(const TArray<float>& V)
{
    double SumSq = 0.0;
    for (float x : V) SumSq += static_cast<double>(x) * static_cast<double>(x);
    return static_cast<float>(FMath::Sqrt(SumSq));
}

void ULabelingDenseProcessorComponent::NormalizeVector(TArray<float>& V)
{
    const float N = VectorNorm(V);
    if (N <= KINDA_SMALL_NUMBER) return;
    for (float& x : V) x /= N;
}

void ULabelingDenseProcessorComponent::PowerIteration(const TArray<float>& Cov, int32 N, int32 MaxIters, float Eps, float& OutEigenvalue, TArray<float>& OutEigenvector)
{
    OutEigenvector.SetNumUninitialized(N);
    // Initialize with random small values
    for (int32 i = 0; i < N; ++i)
    {
        OutEigenvector[i] = FMath::FRandRange(-1.f, 1.f);
    }
    NormalizeVector(OutEigenvector);

    TArray<float> PrevVec;
    PrevVec.SetNumUninitialized(N);

    for (int32 iter = 0; iter < MaxIters; ++iter)
    {
        // y = Cov * v
        TArray<float> Y;
        Y.SetNumZeroed(N);
        for (int32 r = 0; r < N; ++r)
        {
            double Acc = 0.0;
            for (int32 c = 0; c < N; ++c)
            {
                Acc += static_cast<double>(Cov[r * N + c]) * static_cast<double>(OutEigenvector[c]);
            }
            Y[r] = static_cast<float>(Acc);
        }

        PrevVec = OutEigenvector;
        OutEigenvector = Y;
        NormalizeVector(OutEigenvector);

        // Check convergence via angle/difference
        double Diff = 0.0;
        for (int32 i = 0; i < N; ++i)
        {
            const double d = static_cast<double>(OutEigenvector[i]) - static_cast<double>(PrevVec[i]);
            Diff += d * d;
        }
        if (Diff < static_cast<double>(Eps) * static_cast<double>(Eps))
        {
            break;
        }
    }

    // Rayleigh quotient for eigenvalue: v^T C v
    double Lambda = 0.0;
    for (int32 r = 0; r < N; ++r)
    {
        double RowSum = 0.0;
        for (int32 c = 0; c < N; ++c)
        {
            RowSum += static_cast<double>(Cov[r * N + c]) * static_cast<double>(OutEigenvector[c]);
        }
        Lambda += static_cast<double>(OutEigenvector[r]) * RowSum;
    }
    OutEigenvalue = static_cast<float>(Lambda);
}

void ULabelingDenseProcessorComponent::Deflate(TArray<float>& Cov, int32 N, float Eigenvalue, const TArray<float>& Eigenvector)
{
    // Cov := Cov - lambda * v v^T
    for (int32 i = 0; i < N; ++i)
    {
        for (int32 j = 0; j < N; ++j)
        {
            Cov[i * N + j] -= Eigenvalue * Eigenvector[i] * Eigenvector[j];
        }
    }
}

// -------------------- Simple 3-5-2 MLP --------------------

ULabelingDenseProcessorComponent::FMLP352::FMLP352()
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

float ULabelingDenseProcessorComponent::FMLP352::Sigmoid(float X)
{
    return 1.0f / (1.0f + FMath::Exp(-X));
}

float ULabelingDenseProcessorComponent::FMLP352::SigmoidDerivFromY(float Y)
{
    return Y * (1.0f - Y);
}

void ULabelingDenseProcessorComponent::FMLP352::Forward(const float* Input, float* OutHidden, float* OutOutput) const
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

void ULabelingDenseProcessorComponent::FMLP352::TrainSGD(const float* Input, const float* Target, float LearningRate)
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

void ULabelingDenseProcessorComponent::TrainReducedDataLabelingNeuralNetwork(int32 MaxEpochs, float LearningRate, float ErrorThreshold)
{
    // Inputs: 3 features; Outputs: one-hot [C0, C1]
    const float Inputs[3][3] = {
        { 0.5f, 0.3f, 0.2f },
        { 0.7f, 0.6f, 0.5f },
        { 0.9f, 0.8f, 0.7f }
    };
    const float Targets[3][2] = {
        { 1.f, 0.f }, // Class 0
        { 0.f, 1.f }, // Class 1
        { 1.f, 0.f }  // Class 0
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

