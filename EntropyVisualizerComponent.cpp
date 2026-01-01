#include "EntropyVisualizerComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UEntropyVisualizerComponent::UEntropyVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Probabilities = {0.2, 0.3, 0.1, 0.4};
	HeightScale = 10.0f;
}

void UEntropyVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnBars();
}

void UEntropyVisualizerComponent::SpawnBars() const
{
	if (!BarActorClass || !GetWorld())
	{
		return;
	}

	for (int32 Index = 0; Index < Probabilities.Num(); ++Index)
	{
		const float Height = static_cast<float>(Probabilities[Index]) * HeightScale;
		const FVector Location(Index, Height * 0.5f, 0.0f);
		AActor* Bar = GetWorld()->SpawnActor<AActor>(BarActorClass, Location, FRotator::ZeroRotator);
		if (!Bar)
		{
			continue;
		}

		const FVector Scale(0.5f, Height, 0.5f);
		Bar->SetActorScale3D(Scale);
	}
}
