#include "ClassificationManager.h"
#include "Math/UnrealMathUtility.h"

namespace
{
    FORCEINLINE int W1Index(int r, int c, int Hidden, int Input) { return r * Input + c; }
    FORCEINLINE int W2Index(int r, int c, int Output, int Hidden) { return r * Hidden + c; }
}

// ------------------------ NN helpers ------------------------
double UClassificationManager::Sigmoid(double x)
{
    x = FMath::Clamp(x, -50.0, 50.0);
    return 1.0 / (1.0 + FMath::Exp(-x));
}

double UClassificationManager::SigmoidDerivFromActivation(double a)
{
    return a * (1.0 - a);
}

void UClassificationManager::RandomizeParams(double Scale)
{
    W1.SetNumZeroed(HiddenSize * InputSize);
    b1.SetNumZeroed(HiddenSize);
    W2.SetNumZeroed(OutputSize * HiddenSize);
    b2.SetNumZeroed(OutputSize);

    for (int i = 0; i < W1.Num(); ++i) W1[i] = FMath::FRandRange(-Scale, Scale);
    for (int i = 0; i < b1.Num(); ++i) b1[i] = 0.0;
    for (int i = 0; i < W2.Num(); ++i) W2[i] = FMath::FRandRange(-Scale, Scale);
    for (int i = 0; i < b2.Num(); ++i) b2[i] = 0.0;
}

void UClassificationManager::InitializeNetwork(int32 InInputSize, int32 InHiddenNeurons, int32 InOutputSize)
{
    InputSize = InInputSize;
    HiddenSize = InHiddenNeurons;
    OutputSize = InOutputSize;
    RandomizeParams(0.1);
}

void UClassificationManager::ForwardSingle(const double* InX, TArray<double>& Z1, TArray<double>& A1, TArray<double>& Z2, TArray<double>& A2) const
{
    Z1.SetNumUninitialized(HiddenSize);
    A1.SetNumUninitialized(HiddenSize);
    for (int h = 0; h < HiddenSize; ++h)
    {
        double z = b1[h];
        for (int i = 0; i < InputSize; ++i)
        {
            z += W1[W1Index(h, i, HiddenSize, InputSize)] * InX[i];
        }
        Z1[h] = z;
        A1[h] = Sigmoid(z);
    }

    Z2.SetNumUninitialized(OutputSize);
    A2.SetNumUninitialized(OutputSize);
    for (int o = 0; o < OutputSize; ++o)
    {
        double z = b2[o];
        for (int h = 0; h < HiddenSize; ++h)
        {
            z += W2[W2Index(o, h, OutputSize, HiddenSize)] * A1[h];
        }
        Z2[o] = z;
        A2[o] = Sigmoid(z);
    }
}

void UClassificationManager::AddClassificationData(const TArray<double>& Features, int32 Label)
{
    int32 Pred = PredictLabel(Features);
    Dataset.Add(FClassificationData(Features, Label, Pred));
}

double UClassificationManager::TrainNetwork(int32 Epochs, double LearningRate)
{
    if (Dataset.Num() == 0 || InputSize == 0 || HiddenSize == 0 || OutputSize == 0)
    {
        return 0.0;
    }

    const int32 N = Dataset.Num();
    TArray<double> X; X.SetNumUninitialized(N * InputSize);
    TArray<double> Y; Y.SetNumZeroed(N * OutputSize); // one-hot

    for (int32 n = 0; n < N; ++n)
    {
        const FClassificationData& D = Dataset[n];
        for (int i = 0; i < InputSize; ++i)
        {
            X[n * InputSize + i] = (i < D.Features.Num()) ? D.Features[i] : 0.0;
        }
        if (D.Label >= 0 && D.Label < OutputSize)
        {
            Y[n * OutputSize + D.Label] = 1.0;
        }
    }

    TArray<double> Z1, A1, Z2, A2;
    TArray<double> dW1, db1, dW2, db2;
    dW1.SetNumZeroed(W1.Num());
    db1.SetNumZeroed(b1.Num());
    dW2.SetNumZeroed(W2.Num());
    db2.SetNumZeroed(b2.Num());

    double FinalError = 0.0;
    for (int32 epoch = 0; epoch < Epochs; ++epoch)
    {
        FMemory::Memzero(dW1.GetData(), sizeof(double) * dW1.Num());
        FMemory::Memzero(db1.GetData(), sizeof(double) * db1.Num());
        FMemory::Memzero(dW2.GetData(), sizeof(double) * dW2.Num());
        FMemory::Memzero(db2.GetData(), sizeof(double) * db2.Num());

        double SumSqError = 0.0;

        for (int32 n = 0; n < N; ++n)
        {
            const double* x = &X[n * InputSize];
            const double* y = &Y[n * OutputSize];

            ForwardSingle(x, Z1, A1, Z2, A2);

            // Output delta (MSE + sigmoid)
            TArray<double> dA2; dA2.SetNumUninitialized(OutputSize);
            for (int o = 0; o < OutputSize; ++o)
            {
                const double err = A2[o] - y[o];
                SumSqError += 0.5 * err * err;
                dA2[o] = err * SigmoidDerivFromActivation(A2[o]);
            }

            // Grad W2, b2
            for (int o = 0; o < OutputSize; ++o)
            {
                db2[o] += dA2[o];
                for (int h = 0; h < HiddenSize; ++h)
                {
                    dW2[W2Index(o, h, OutputSize, HiddenSize)] += dA2[o] * A1[h];
                }
            }

            // Backprop to hidden
            TArray<double> dA1; dA1.SetNumUninitialized(HiddenSize);
            for (int h = 0; h < HiddenSize; ++h)
            {
                double grad = 0.0;
                for (int o = 0; o < OutputSize; ++o)
                {
                    grad += dA2[o] * W2[W2Index(o, h, OutputSize, HiddenSize)];
                }
                dA1[h] = grad * SigmoidDerivFromActivation(A1[h]);
            }

            // Grad W1, b1
            for (int h = 0; h < HiddenSize; ++h)
            {
                db1[h] += dA1[h];
                for (int i = 0; i < InputSize; ++i)
                {
                    dW1[W1Index(h, i, HiddenSize, InputSize)] += dA1[h] * x[i];
                }
            }
        }

        const double InvN = 1.0 / FMath::Max(1, N);
        for (int i = 0; i < dW1.Num(); ++i) W1[i] -= LearningRate * dW1[i] * InvN;
        for (int i = 0; i < db1.Num(); ++i) b1[i] -= LearningRate * db1[i] * InvN;
        for (int i = 0; i < dW2.Num(); ++i) W2[i] -= LearningRate * dW2[i] * InvN;
        for (int i = 0; i < db2.Num(); ++i) b2[i] -= LearningRate * db2[i] * InvN;

        FinalError = SumSqError * InvN;
        if (epoch % 100 == 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Epoch %d: Error = %f"), epoch, FinalError);
        }
    }

    return FinalError;
}

int32 UClassificationManager::PredictLabel(const TArray<double>& Features) const
{
    if (InputSize == 0 || HiddenSize == 0 || OutputSize == 0)
    {
        return -1;
    }

    // Build input vector
    TArray<double> X; X.SetNumZeroed(InputSize);
    for (int i = 0; i < InputSize && i < Features.Num(); ++i)
    {
        X[i] = Features[i];
    }

    TArray<double> Z1, A1, Z2, A2;
    ForwardSingle(X.GetData(), Z1, A1, Z2, A2);

    // Argmax
    int32 BestIdx = 0;
    double BestVal = A2[0];
    for (int32 o = 1; o < OutputSize; ++o)
    {
        if (A2[o] > BestVal) { BestVal = A2[o]; BestIdx = o; }
    }
    return BestIdx;
}

// ------------------------ KMeans ------------------------
void UClassificationManager::InitializeKMeans(int32 InClusterCount)
{
    ClusterCount = InClusterCount;
    KM_Dim = 0;
    KM_Centroids.Reset();
    bKM_Trained = false;
}

double UClassificationManager::SqDistance(const double* A, const double* B, int32 Dim)
{
    double s = 0.0;
    for (int i = 0; i < Dim; ++i)
    {
        const double d = A[i] - B[i];
        s += d * d;
    }
    return s;
}

void UClassificationManager::TrainKMeans(const TArray<FDoubleArray>& FeatureSet, int32 MaxIters)
{
    if (ClusterCount <= 0 || FeatureSet.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("KMeans not initialized or empty feature set."));
        bKM_Trained = false;
        return;
    }

    KM_Dim = FeatureSet[0].Values.Num();
    const int32 N = FeatureSet.Num();
    if (KM_Dim == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("KMeans: zero-dim features."));
        bKM_Trained = false;
        return;
    }

    // Initialize centroids: pick first K samples (or wrap)
    KM_Centroids.SetNumUninitialized(ClusterCount * KM_Dim);
    for (int32 k = 0; k < ClusterCount; ++k)
    {
        const TArray<double>& src = FeatureSet[k % N].Values;
        for (int d = 0; d < KM_Dim; ++d)
        {
            KM_Centroids[k * KM_Dim + d] = (d < src.Num()) ? src[d] : 0.0;
        }
    }

    TArray<int32> Labels; Labels.Init(0, N);
    TArray<double> NewCentroids; NewCentroids.SetNumZeroed(ClusterCount * KM_Dim);
    TArray<int32> Counts; Counts.Init(0, ClusterCount);

    for (int iter = 0; iter < MaxIters; ++iter)
    {
        // Assignment step
        for (int32 n = 0; n < N; ++n)
        {
            const TArray<double>& x = FeatureSet[n].Values;
            // find nearest centroid
            int32 bestK = 0; double bestD = TNumericLimits<double>::Max();
            for (int32 k = 0; k < ClusterCount; ++k)
            {
                const double* c = &KM_Centroids[k * KM_Dim];
                double d = 0.0;
                for (int dim = 0; dim < KM_Dim; ++dim)
                {
                    const double diff = ((dim < x.Num()) ? x[dim] : 0.0) - c[dim];
                    d += diff * diff;
                }
                if (d < bestD) { bestD = d; bestK = k; }
            }
            Labels[n] = bestK;
        }

        // Update step
        FMemory::Memzero(NewCentroids.GetData(), sizeof(double) * NewCentroids.Num());
        for (int32 k = 0; k < ClusterCount; ++k) Counts[k] = 0;

        for (int32 n = 0; n < N; ++n)
        {
            const int32 k = Labels[n];
            const TArray<double>& x = FeatureSet[n].Values;
            for (int dim = 0; dim < KM_Dim; ++dim)
            {
                NewCentroids[k * KM_Dim + dim] += (dim < x.Num()) ? x[dim] : 0.0;
            }
            Counts[k] += 1;
        }

        // finalize means; track movement
        double Move = 0.0;
        for (int32 k = 0; k < ClusterCount; ++k)
        {
            if (Counts[k] > 0)
            {
                for (int dim = 0; dim < KM_Dim; ++dim)
                {
                    const double newVal = NewCentroids[k * KM_Dim + dim] / (double)Counts[k];
                    const double oldVal = KM_Centroids[k * KM_Dim + dim];
                    Move += FMath::Abs(newVal - oldVal);
                    KM_Centroids[k * KM_Dim + dim] = newVal;
                }
            }
        }

        if (Move < 1e-6)
        {
            break; // converged
        }
    }

    bKM_Trained = true;
}

int32 UClassificationManager::PredictCluster(const TArray<double>& Features) const
{
    if (!bKM_Trained || ClusterCount <= 0 || KM_Dim <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("KMeans clustering is not initialized. Returning -1."));
        return -1;
    }

    // Argmin distance to centroids
    int32 bestK = 0; double bestD = TNumericLimits<double>::Max();
    for (int32 k = 0; k < ClusterCount; ++k)
    {
        const double* c = &KM_Centroids[k * KM_Dim];
        double d = 0.0;
        for (int dim = 0; dim < KM_Dim; ++dim)
        {
            const double diff = ((dim < Features.Num()) ? Features[dim] : 0.0) - c[dim];
            d += diff * diff;
        }
        if (d < bestD) { bestD = d; bestK = k; }
    }
    return bestK;
}
