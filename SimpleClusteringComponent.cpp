#include "SimpleClusteringComponent.h"

#include "Math/UnrealMathUtility.h"

USimpleClusteringComponent::USimpleClusteringComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USimpleClusteringComponent::BeginPlay()
{
    Super::BeginPlay();

    DataPoints = GenerateRandomData(100);
    Centroids = InitializeCentroids(K);

    ClusterAssignments.Empty();
    for (int32 i = 0; i < K; ++i)
    {
        ClusterAssignments.Add(i, FVector2DArray());
    }

    for (int32 iter = 0; iter < 10; ++iter)
    {
        AssignClusters();
        RecalculateCentroids();
    }

    UE_LOG(LogTemp, Log, TEXT("Clustering performed."));
}

TArray<FVector2D> USimpleClusteringComponent::GenerateRandomData(int32 Count) const
{
    TArray<FVector2D> Data; Data.SetNum(FMath::Max(0, Count));
    for (int32 i = 0; i < Data.Num(); ++i)
    {
        Data[i] = FVector2D(FMath::FRand(), FMath::FRand());
    }
    return Data;
}

TArray<FVector2D> USimpleClusteringComponent::InitializeCentroids(int32 InK) const
{
    TArray<FVector2D> Centers; Centers.SetNum(FMath::Max(0, InK));
    for (int32 i = 0; i < Centers.Num(); ++i)
    {
        Centers[i] = FVector2D(FMath::FRand(), FMath::FRand());
    }
    return Centers;
}

void USimpleClusteringComponent::AssignClusters()
{
    // Assign each data point to the nearest centroid (accumulates like the C# sample)
    for (const FVector2D& Point : DataPoints)
    {
        const int32 Closest = FindClosestCentroid(Point);
        FVector2DArray& Bucket = ClusterAssignments.FindOrAdd(Closest);
        Bucket.Values.Add(Point);
    }
}

void USimpleClusteringComponent::RecalculateCentroids()
{
    for (int32 i = 0; i < Centroids.Num(); ++i)
    {
        const FVector2DArray* Cluster = ClusterAssignments.Find(i);
        if (!Cluster || Cluster->Values.Num() == 0)
        {
            continue;
        }

        double SumX = 0.0, SumY = 0.0;
        for (const FVector2D& P : Cluster->Values)
        {
            SumX += P.X;
            SumY += P.Y;
        }
        const double Cnt = static_cast<double>(Cluster->Values.Num());
        Centroids[i] = FVector2D(static_cast<float>(SumX / Cnt), static_cast<float>(SumY / Cnt));
    }
}

int32 USimpleClusteringComponent::FindClosestCentroid(const FVector2D& Point) const
{
    int32 ClosestIndex = 0;
    float MinDistance = TNumericLimits<float>::Max();

    for (int32 i = 0; i < Centroids.Num(); ++i)
    {
        const FVector2D& C = Centroids[i];
        const float Dx = Point.X - C.X;
        const float Dy = Point.Y - C.Y;
        const float Dist = FMath::Sqrt(Dx * Dx + Dy * Dy);
        if (Dist < MinDistance)
        {
            MinDistance = Dist;
            ClosestIndex = i;
        }
    }

    return ClosestIndex;
}
