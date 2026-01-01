#include "HypervectorClusterComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

namespace HypervectorClusterImpl
{
	struct FClusterInternal
	{
		FHypervectorPoint Centroid;
		TArray<int32> VectorIndices;
	};

	float CalculateCosineSimilarity(const FHypervectorPoint& A, const FHypervectorPoint& B)
	{
		if (!A.IsValid() || !B.IsValid() || A.Num() != B.Num())
		{
			return 0.0f;
		}

		float Dot = 0.0f;
		float NormA = 0.0f;
		float NormB = 0.0f;
		for (int32 Index = 0; Index < A.Num(); ++Index)
		{
			const float ValueA = A.Values[Index];
			const float ValueB = B.Values[Index];
			Dot += ValueA * ValueB;
			NormA += ValueA * ValueA;
			NormB += ValueB * ValueB;
		}

		NormA = FMath::Sqrt(NormA);
		NormB = FMath::Sqrt(NormB);
		if (NormA <= KINDA_SMALL_NUMBER || NormB <= KINDA_SMALL_NUMBER)
		{
			return 0.0f;
		}
		return Dot / (NormA * NormB);
	}

	void UpdateCentroid(FClusterInternal& Cluster, const TArray<FHypervectorPoint>& Vectors, int32 Dimension)
	{
		if (Cluster.VectorIndices.Num() == 0 || Dimension <= 0)
		{
			return;
		}

		Cluster.Centroid = FHypervectorPoint(Dimension);
		for (int32 Index : Cluster.VectorIndices)
		{
			if (!Vectors.IsValidIndex(Index))
			{
				continue;
			}

			const FHypervectorPoint& Source = Vectors[Index];
			for (int32 DimIndex = 0; DimIndex < Dimension; ++DimIndex)
			{
				Cluster.Centroid.Values[DimIndex] += Source.Values[DimIndex];
			}
		}

		for (int32 DimIndex = 0; DimIndex < Dimension; ++DimIndex)
		{
			Cluster.Centroid.Values[DimIndex] /= Cluster.VectorIndices.Num();
		}
	}
}

UHypervectorClusterComponent::UHypervectorClusterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FHypervectorClusterResult> UHypervectorClusterComponent::ClusterHypervectors(const TArray<FHypervectorPoint>& Vectors, int32 NumClusters, int32 MaxIterations) const
{
	TArray<FHypervectorClusterResult> Results;
	if (NumClusters <= 0 || Vectors.Num() == 0)
	{
		return Results;
	}

	const int32 Dimension = Vectors[0].Num();
	TArray<HypervectorClusterImpl::FClusterInternal> Clusters;
	Clusters.SetNumZeroed(NumClusters);

	FRandomStream RandomStream(static_cast<int32>(FDateTime::Now().GetTicks()));
	for (int32 Index = 0; Index < Vectors.Num(); ++Index)
	{
		const int32 ClusterIndex = RandomStream.RandRange(0, NumClusters - 1);
		Clusters[ClusterIndex].VectorIndices.Add(Index);
	}

	MaxIterations = FMath::Max(MaxIterations, 1);
	for (int32 Iteration = 0; Iteration < MaxIterations; ++Iteration)
	{
		for (HypervectorClusterImpl::FClusterInternal& Cluster : Clusters)
		{
			HypervectorClusterImpl::UpdateCentroid(Cluster, Vectors, Dimension);
		}

		for (HypervectorClusterImpl::FClusterInternal& Cluster : Clusters)
		{
			Cluster.VectorIndices.Reset();
		}

		for (int32 VectorIndex = 0; VectorIndex < Vectors.Num(); ++VectorIndex)
		{
			const FHypervectorPoint& Source = Vectors[VectorIndex];
			int32 BestCluster = 0;
			float BestSimilarity = -1.0f;

			for (int32 ClusterIndex = 0; ClusterIndex < Clusters.Num(); ++ClusterIndex)
			{
				const float Similarity = HypervectorClusterImpl::CalculateCosineSimilarity(Source, Clusters[ClusterIndex].Centroid);
				if (Similarity > BestSimilarity)
				{
					BestSimilarity = Similarity;
					BestCluster = ClusterIndex;
				}
			}

			Clusters[BestCluster].VectorIndices.Add(VectorIndex);
		}
	}

	for (int32 ClusterIndex = 0; ClusterIndex < Clusters.Num(); ++ClusterIndex)
	{
		HypervectorClusterImpl::UpdateCentroid(Clusters[ClusterIndex], Vectors, Dimension);

		FHypervectorClusterResult Result;
		Result.ClusterIndex = ClusterIndex;
		Result.Centroid = Clusters[ClusterIndex].Centroid;
		Result.VectorIndices = Clusters[ClusterIndex].VectorIndices;
		Results.Add(Result);
	}

	return Results;
}
