#include "ClusteringLibrary.h"

#include "Math/UnrealMathUtility.h"

void UClusteringLibrary::KMeansCluster(const TArray<FFloatArray>& Data, int32 K, int32 MaxIterations, TArray<FFloatArrayList>& OutClusters)
{
    OutClusters.Reset();

    const int32 N = Data.Num();
    if (N == 0 || K <= 0)
    {
        return;
    }

    const int32 Dim = Data[0].Values.Num();
    if (Dim == 0)
    {
        return;
    }

    // Initialize centroids by randomly selecting K points (with replacement if K > N)
    TArray<TArray<float>> Centroids;
    Centroids.SetNum(K);
    for (int32 i = 0; i < K; ++i)
    {
        const int32 idx = FMath::RandRange(0, FMath::Max(0, N - 1));
        Centroids[i] = Data[idx].Values; // copy
    }

    TArray<FFloatArrayList> Clusters;
    Clusters.SetNum(K);

    for (int32 iter = 0; iter < FMath::Max(1, MaxIterations); ++iter)
    {
        // Reset clusters
        for (int32 i = 0; i < K; ++i)
        {
            Clusters[i].Values.Reset();
        }

        // Assign points to nearest centroid
        for (const FFloatArray& Point : Data)
        {
            int32 Nearest = 0;
            float MinDist = TNumericLimits<float>::Max();
            for (int32 i = 0; i < K; ++i)
            {
                const float D = EuclideanDistance(Point.Values, Centroids[i]);
                if (D < MinDist)
                {
                    MinDist = D;
                    Nearest = i;
                }
            }
            Clusters[Nearest].Values.Add(Point);
        }

        // Recompute centroids
        for (int32 i = 0; i < K; ++i)
        {
            if (Clusters[i].Values.Num() > 0)
            {
                TArray<float> Mean; CalculateMean(Clusters[i].Values, Mean);
                Centroids[i] = MoveTemp(Mean);
            }
            // else keep previous centroid
        }
    }

    OutClusters = MoveTemp(Clusters);
}

float UClusteringLibrary::EuclideanDistance(const TArray<float>& A, const TArray<float>& B)
{
    const int32 LA = A.Num();
    const int32 LB = B.Num();
    const int32 D = FMath::Min(LA, LB);
    float Sum = 0.0f;
    for (int32 i = 0; i < D; ++i)
    {
        const float Diff = A[i] - B[i];
        Sum += Diff * Diff;
    }
    // If dimensions mismatch, ignore extra components (mirrors simple guard)
    return FMath::Sqrt(Sum);
}

void UClusteringLibrary::CalculateMean(const TArray<FFloatArray>& Points, TArray<float>& OutMean)
{
    OutMean.Reset();
    if (Points.Num() == 0) { return; }
    const int32 Dim = Points[0].Values.Num();
    if (Dim == 0) { return; }

    OutMean.Init(0.0f, Dim);
    for (const FFloatArray& P : Points)
    {
        for (int32 i = 0; i < Dim; ++i)
        {
            OutMean[i] += (i < P.Values.Num()) ? P.Values[i] : 0.0f;
        }
    }
    const float Inv = 1.0f / static_cast<float>(Points.Num());
    for (int32 i = 0; i < Dim; ++i)
    {
        OutMean[i] *= Inv;
    }
}

