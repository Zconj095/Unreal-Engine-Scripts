// Copyright Epic Games, Inc.

#include "GroverKMeansComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UGroverKMeansComponent::UGroverKMeansComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGroverKMeansComponent::RunQuantumKMeans()
{
	InitializeDataPoints();
	InitializeCentroids();

	for (int32 Iteration = 0; Iteration < NumIterations; ++Iteration)
	{
		if (bLogIntermediateSteps)
		{
			UE_LOG(LogTemp, Log, TEXT("Iteration %d"), Iteration + 1);
		}

		AssignClusters();
		UpdateCentroids();

		if (bLogIntermediateSteps)
		{
			LogState(FString::Printf(TEXT("After Iteration %d"), Iteration + 1));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Quantum K-Means Completed."));
}

void UGroverKMeansComponent::InitializeDataPoints()
{
	DataPoints.SetNum(NumPoints);
	ClusterAssignments.SetNum(NumPoints);

	for (int32 i = 0; i < NumPoints; ++i)
	{
		const float X = UKismetMathLibrary::RandomFloatInRange(DataRange.X, DataRange.Y);
		const float Y = UKismetMathLibrary::RandomFloatInRange(DataRange.X, DataRange.Y);
		DataPoints[i] = FVector2D(X, Y);
		ClusterAssignments[i] = 0;
	}

	VisualizeDataPoints();
}

void UGroverKMeansComponent::InitializeCentroids()
{
	Centroids.SetNum(NumClusters);
	for (int32 i = 0; i < NumClusters; ++i)
	{
		const float X = UKismetMathLibrary::RandomFloatInRange(DataRange.X, DataRange.Y);
		const float Y = UKismetMathLibrary::RandomFloatInRange(DataRange.X, DataRange.Y);
		Centroids[i] = FVector2D(X, Y);
	}

	VisualizeCentroids();
}

void UGroverKMeansComponent::AssignClusters()
{
	for (int32 i = 0; i < DataPoints.Num(); ++i)
	{
		ClusterAssignments[i] = FindClosestCentroid(DataPoints[i]);
	}
}

int32 UGroverKMeansComponent::FindClosestCentroid(const FVector2D& Point) const
{
	int32 ClosestCentroid = 0;
	float MinDistance = TNumericLimits<float>::Max();

	for (int32 i = 0; i < Centroids.Num(); ++i)
	{
		const float Distance = FVector2D::Distance(Point, Centroids[i]);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestCentroid = i;
		}
	}

	if (bLogIntermediateSteps)
	{
		UE_LOG(LogTemp, Log, TEXT("Point (%f, %f) assigned to Centroid %d"), Point.X, Point.Y, ClosestCentroid);
	}

	return ClosestCentroid;
}

void UGroverKMeansComponent::UpdateCentroids()
{
	TArray<FVector2D> NewCentroids;
	NewCentroids.SetNum(NumClusters);
	TArray<int32> ClusterSizes;
	ClusterSizes.Init(0, NumClusters);

	for (int32 i = 0; i < DataPoints.Num(); ++i)
	{
		const int32 Cluster = ClusterAssignments[i];
		if (NewCentroids.IsValidIndex(Cluster))
		{
			NewCentroids[Cluster] += DataPoints[i];
			ClusterSizes[Cluster]++;
		}
	}

	for (int32 i = 0; i < NumClusters; ++i)
	{
		if (ClusterSizes[i] > 0)
		{
			NewCentroids[i] /= static_cast<float>(ClusterSizes[i]);
		}
		else if (Centroids.IsValidIndex(i))
		{
			NewCentroids[i] = Centroids[i];
		}
	}

	Centroids = NewCentroids;
	VisualizeCentroids();
}

void UGroverKMeansComponent::VisualizeDataPoints()
{
	if (bClearPreviousVisuals)
	{
		for (TWeakObjectPtr<AActor>& ActorPtr : PointActors)
		{
			if (ActorPtr.IsValid())
			{
				ActorPtr->Destroy();
			}
		}
		PointActors.Reset();
	}

	if (!PointActorClass || !GetWorld())
	{
		return;
	}

	for (const FVector2D& Point : DataPoints)
	{
		const FVector Location(Point.X, Point.Y, PointHeight);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(PointActorClass, Location, FRotator::ZeroRotator, Params);
		if (Spawned)
		{
			PointActors.Add(Spawned);
		}
	}
}

void UGroverKMeansComponent::VisualizeCentroids()
{
	if (bClearPreviousVisuals)
	{
		for (TWeakObjectPtr<AActor>& ActorPtr : CentroidActors)
		{
			if (ActorPtr.IsValid())
			{
				ActorPtr->Destroy();
			}
		}
		CentroidActors.Reset();
	}

	if (!CentroidActorClass || !GetWorld())
	{
		return;
	}

	for (const FVector2D& Centroid : Centroids)
	{
		const FVector Location(Centroid.X, Centroid.Y, PointHeight);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(CentroidActorClass, Location, FRotator::ZeroRotator, Params);
		if (Spawned)
		{
			CentroidActors.Add(Spawned);
		}
	}
}

void UGroverKMeansComponent::ClearVisuals()
{
	for (TWeakObjectPtr<AActor>& ActorPtr : PointActors)
	{
		if (ActorPtr.IsValid())
		{
			ActorPtr->Destroy();
		}
	}
	PointActors.Reset();

	for (TWeakObjectPtr<AActor>& ActorPtr : CentroidActors)
	{
		if (ActorPtr.IsValid())
		{
			ActorPtr->Destroy();
		}
	}
	CentroidActors.Reset();
}

void UGroverKMeansComponent::LogState(const FString& Message) const
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	for (int32 i = 0; i < Centroids.Num(); ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Centroid %d: (%f, %f)"), i, Centroids[i].X, Centroids[i].Y);
	}

	for (int32 i = 0; i < DataPoints.Num(); ++i)
	{
		if (ClusterAssignments.IsValidIndex(i))
		{
			UE_LOG(LogTemp, Log, TEXT("Point (%f, %f) -> Cluster %d"), DataPoints[i].X, DataPoints[i].Y, ClusterAssignments[i]);
		}
	}
}
