#include "EnergyTransferVisualizerComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UEnergyTransferVisualizerComponent::UEnergyTransferVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	StartTime = 0.0f;
	EndTime = PI;
	Step = 0.1f;
}

void UEnergyTransferVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
	VisualizeTemporalPower();
}

void UEnergyTransferVisualizerComponent::VisualizeTemporalPower() const
{
	if (!PointActorClass || !GetWorld())
	{
		return;
	}

	for (float Time = StartTime; Time <= EndTime; Time += Step)
	{
		const float TemporalFlow = FMath::Sin(Time);
		const float LeylineGradient = FMath::Cos(Time);
		const float Power = TemporalFlow * LeylineGradient;
		const FVector Position(Time, Power, 0.0f);

		AActor* Actor = GetWorld()->SpawnActor<AActor>(PointActorClass, Position, FRotator::ZeroRotator);
		if (Actor)
		{
			UE_LOG(LogTemp, Log, TEXT("Time: %.2f, Power: %.4f"), Time, Power);
		}
	}
}
