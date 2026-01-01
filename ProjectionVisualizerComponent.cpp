#include "ProjectionVisualizerComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Logging/LogMacros.h"

UProjectionVisualizerComponent::UProjectionVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectionVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
	VisualizeProjection(1.0f);
}

void UProjectionVisualizerComponent::VisualizeProjection(float TimeSeconds)
{
	if (!CubeActor)
	{
		UE_LOG(LogTemp, Error, TEXT("CubeActor is not set."));
		return;
	}

	if (GridSize <= 0)
	{
		return;
	}

	// Simulate projection via simple procedural pattern
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (int32 x = 0; x < GridSize; ++x)
	{
		for (int32 y = 0; y < GridSize; ++y)
		{
			for (int32 z = 0; z < GridSize; ++z)
			{
				const float Strength = FMath::Sin((x + y + z + TimeSeconds) * 0.1f);
				if (Strength > Threshold)
				{
					const FVector BaseLocation = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
					AActor* Spawned = World->SpawnActor<AActor>(CubeActor, BaseLocation + FVector(x, y, z), FRotator::ZeroRotator);
					if (Spawned)
					{
						Spawned->SetActorScale3D(FVector(0.5f));
					}
				}
			}
		}
	}
}
