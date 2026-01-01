#include "PopulationVisualizerComponent.h"
#include "Engine/World.h"
#include "Logging/LogMacros.h"
#include "GameFramework/Actor.h"

UPopulationVisualizerComponent::UPopulationVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPopulationVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!PointActor)
	{
		UE_LOG(LogTemp, Error, TEXT("PointActor must be set for visualization."));
		return;
	}

	if (EnvironmentalPressures.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No environmental pressures supplied."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TArray<float> PopulationHistory;
	PopulationHistory.SetNumZeroed(EnvironmentalPressures.Num());

	float CurrentPopulation = InitialPopulation;
	for (int32 Index = 0; Index < EnvironmentalPressures.Num(); ++Index)
	{
		float Pressure = EnvironmentalPressures[Index];
		CurrentPopulation += TimeStep * (Alpha * CurrentPopulation * (1.0f - CurrentPopulation / CarryingCapacity) + Beta * Pressure);
		PopulationHistory[Index] = CurrentPopulation;
	}

	for (int32 Index = 0; Index < PopulationHistory.Num(); ++Index)
	{
		const float Value = PopulationHistory[Index];
		const FVector Location = FVector(Index, Value, 0.0f) + (GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector);
		AActor* Spawned = World->SpawnActor<AActor>(PointActor, Location, FRotator::ZeroRotator);
		if (Spawned)
		{
			Spawned->SetActorScale3D(FVector(0.5f));
		}
	}
}
