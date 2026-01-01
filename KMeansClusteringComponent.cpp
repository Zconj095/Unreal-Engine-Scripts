#include "KMeansClusteringComponent.h"

#include "Logging/LogMacros.h"

UKMeansClusteringComponent::UKMeansClusteringComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UKMeansClusteringComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataPoints.Num() == 0)
	{
		InitializeExampleData();
	}

	RunClustering();
}

void UKMeansClusteringComponent::RunClustering()
{
	if (DataPoints.Num() == 0 || K <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("KMeans: No data or invalid K."));
		return;
	}

	Centroids = PerformKMeansClustering(DataPoints, K);

	UE_LOG(LogTemp, Log, TEXT("Final Centroids:"));
	for (const FKMeansDataPoint& C : Centroids)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *C.ToString());
	}
}

TArray<FKMeansDataPoint> UKMeansClusteringComponent::PerformKMeansClustering(const TArray<FKMeansDataPoint>& Data, int32 InK)
{
	TArray<FKMeansDataPoint> WorkingData = Data;
	TArray<FKMeansDataPoint> LocalCentroids = InitializeCentroids(WorkingData, InK);

	bool bCentroidsChanged = true;

	while (bCentroidsChanged)
	{
		bCentroidsChanged = false;

		// Assign to nearest centroid
		for (FKMeansDataPoint& Point : WorkingData)
		{
			Point.AssignedCluster = GetClosestCentroid(Point, LocalCentroids);
		}

		// Recompute centroids
		for (int32 ClusterIdx = 0; ClusterIdx < InK; ++ClusterIdx)
		{
			TArray<FKMeansDataPoint> ClusterPoints;
			for (const FKMeansDataPoint& P : WorkingData)
			{
				if (P.AssignedCluster == ClusterIdx)
				{
					ClusterPoints.Add(P);
				}
			}

			if (ClusterPoints.Num() > 0)
			{
				FKMeansDataPoint NewCentroid = CalculateCentroid(ClusterPoints);
				if (!NewCentroid.Equals(LocalCentroids[ClusterIdx]))
				{
					bCentroidsChanged = true;
				}
				LocalCentroids[ClusterIdx] = NewCentroid;
			}
		}
	}

	return LocalCentroids;
}

TArray<FKMeansDataPoint> UKMeansClusteringComponent::InitializeCentroids(const TArray<FKMeansDataPoint>& Data, int32 InK) const
{
	TArray<FKMeansDataPoint> CentroidList;
	if (Data.Num() == 0)
	{
		return CentroidList;
	}

	TArray<int32> UsedIndices;
	while (CentroidList.Num() < InK && UsedIndices.Num() < Data.Num())
	{
		const int32 Index = FMath::RandRange(0, Data.Num() - 1);
		if (!UsedIndices.Contains(Index))
		{
			UsedIndices.Add(Index);
			CentroidList.Add(Data[Index]);
		}
	}

	// If duplicates prevented filling K, repeat until filled.
	while (CentroidList.Num() < InK)
	{
		CentroidList.Add(Data[FMath::RandRange(0, Data.Num() - 1)]);
	}

	return CentroidList;
}

int32 UKMeansClusteringComponent::GetClosestCentroid(const FKMeansDataPoint& Point, const TArray<FKMeansDataPoint>& InCentroids) const
{
	float MinDistance = TNumericLimits<float>::Max();
	int32 Closest = 0;

	for (int32 i = 0; i < InCentroids.Num(); ++i)
	{
		const float Dist = CalculateEuclideanDistance(Point, InCentroids[i]);
		if (Dist < MinDistance)
		{
			MinDistance = Dist;
			Closest = i;
		}
	}

	return Closest;
}

float UKMeansClusteringComponent::CalculateEuclideanDistance(const FKMeansDataPoint& A, const FKMeansDataPoint& B) const
{
	const int32 NumFeat = FMath::Min(A.Features.Num(), B.Features.Num());
	float Sum = 0.f;
	for (int32 i = 0; i < NumFeat; ++i)
	{
		const float Diff = A.Features[i] - B.Features[i];
		Sum += Diff * Diff;
	}
	return FMath::Sqrt(Sum);
}

FKMeansDataPoint UKMeansClusteringComponent::CalculateCentroid(const TArray<FKMeansDataPoint>& Points) const
{
	const int32 NumFeatures = Points.Num() > 0 ? Points[0].Features.Num() : 0;
	FKMeansDataPoint Centroid;
	Centroid.Features.Init(0.f, NumFeatures);

	for (const FKMeansDataPoint& P : Points)
	{
		for (int32 i = 0; i < NumFeatures; ++i)
		{
			Centroid.Features[i] += P.Features[i];
		}
	}

	for (int32 i = 0; i < NumFeatures; ++i)
	{
		Centroid.Features[i] /= Points.Num();
	}

	return Centroid;
}

void UKMeansClusteringComponent::InitializeExampleData()
{
	DataPoints = {
		FKMeansDataPoint({1.0f, 2.0f}),
		FKMeansDataPoint({1.5f, 1.8f}),
		FKMeansDataPoint({5.0f, 8.0f}),
		FKMeansDataPoint({8.0f, 8.0f}),
		FKMeansDataPoint({1.0f, 0.6f}),
		FKMeansDataPoint({9.0f, 11.0f}),
		FKMeansDataPoint({8.0f, 4.0f}),
		FKMeansDataPoint({6.0f, 4.0f}),
		FKMeansDataPoint({3.0f, 7.0f}),
		FKMeansDataPoint({3.5f, 6.5f})
	};
}
