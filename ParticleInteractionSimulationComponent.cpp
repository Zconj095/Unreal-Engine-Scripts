#include "ParticleInteractionSimulationComponent.h"
#include "Logging/LogMacros.h"

UParticleInteractionSimulationComponent::UParticleInteractionSimulationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UParticleInteractionSimulationComponent::BeginPlay()
{
	Super::BeginPlay();

	FinalEnergy = CalculateFinalEnergy();
	UE_LOG(LogTemp, Log, TEXT("Initial Energy (E_initial): %0.4f"), InitialEnergy);
	UE_LOG(LogTemp, Log, TEXT("Magical Energy Contribution (DeltaE_magic): %0.4f"), MagicalContribution);
	UE_LOG(LogTemp, Log, TEXT("Final Energy (E_final): %0.4f"), FinalEnergy);
}

float UParticleInteractionSimulationComponent::CalculateFinalEnergy() const
{
	return InitialEnergy - MagicalContribution;
}
