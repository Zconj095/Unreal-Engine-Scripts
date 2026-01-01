#include "PollutionNeutralizationComponent.h"
#include "Logging/LogMacros.h"

UPollutionNeutralizationComponent::UPollutionNeutralizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPollutionNeutralizationComponent::BeginPlay()
{
	Super::BeginPlay();

	NeutralizationEnergy = CalculateNeutralizationEnergy(100.0f, 50.0f);
	UE_LOG(LogTemp, Log, TEXT("Calculated Neutralization Energy (E_n): %0.2f kJ"), NeutralizationEnergy);
}

float UPollutionNeutralizationComponent::CalculateNeutralizationEnergy(float EnthalpyChange, float MagicalEnergy) const
{
	return EnthalpyChange + MagicalEnhancementFactor * MagicalEnergy;
}
