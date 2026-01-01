#include "ParticleInteractionEfficiencyComponent.h"
#include "Logging/LogMacros.h"

UParticleInteractionEfficiencyComponent::UParticleInteractionEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UParticleInteractionEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	Efficiency = CalculateInteractionEfficiency();
	UE_LOG(LogTemp, Log, TEXT("Particle Interaction Efficiency (η_p): %0.4f"), Efficiency);
}

float UParticleInteractionEfficiencyComponent::CalculateInteractionEfficiency() const
{
	if (FMath::IsNearlyZero(DeltaE))
	{
		UE_LOG(LogTemp, Warning, TEXT("Energy absorbed (ΔE) is zero. Cannot compute efficiency."));
		return 0.0f;
	}

	return DeltaW / DeltaE;
}
