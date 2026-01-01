#include "EmotionalEnergyFlowComponent.h"

UEmotionalEnergyFlowComponent::UEmotionalEnergyFlowComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DeltaEnergy = 10.0f;
	TimeInterval = 2.0f;
	EmotionalEnergyFlow = 0.0f;
}

void UEmotionalEnergyFlowComponent::BeginPlay()
{
	Super::BeginPlay();

	EmotionalEnergyFlow = CalculateEmotionalEnergyFlow(DeltaEnergy, TimeInterval);
	UE_LOG(LogTemp, Log, TEXT("Emotional Energy Flow (F_e): %f"), EmotionalEnergyFlow);
}

float UEmotionalEnergyFlowComponent::CalculateEmotionalEnergyFlow(float DeltaQe, float DeltaT) const
{
	if (FMath::IsNearlyZero(DeltaT))
	{
		UE_LOG(LogTemp, Warning, TEXT("Time interval (Δt) is zero. Cannot compute energy flow."));
		return 0.0f;
	}

	return DeltaQe / DeltaT;
}
