#include "EnvironmentalSystemVisualizerComponent.h"
#include "Kismet/GameplayStatics.h"

UEnvironmentalSystemVisualizerComponent::UEnvironmentalSystemVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Weights = {0.5f, 0.3f, 0.2f};
	Intensities = {100.0f, 200.0f, 150.0f};
}

void UEnvironmentalSystemVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	float SystemStrength = 0.0f;
	for (int32 Index = 0; Index < Weights.Num() && Index < Intensities.Num(); ++Index)
	{
		SystemStrength += Weights[Index] * Intensities[Index];
	}

	LogSystemStrength(SystemStrength);
	VisualizeFactors();
}

void UEnvironmentalSystemVisualizerComponent::VisualizeFactors() const
{
	if (!GetWorld() || !FactorActorClass)
	{
		return;
	}

	for (int32 Index = 0; Index < Intensities.Num(); ++Index)
	{
		const FVector Location(Index * 200.0f, 0.0f, 0.0f);
		AActor* Actor = GetWorld()->SpawnActor<AActor>(FactorActorClass, Location, FRotator::ZeroRotator);
		if (Actor)
		{
			Actor->SetActorLabel(FString::Printf(TEXT("Factor_%d"), Index + 1));
		}
	}
}

void UEnvironmentalSystemVisualizerComponent::LogSystemStrength(float SystemStrength) const
{
	UE_LOG(LogTemp, Log, TEXT("Total System Interaction Strength (S): %.2f"), SystemStrength);
}
