#include "NeuroadaptiveEvolutionIndexComponent.h"
#include "Logging/LogMacros.h"

UNeuroadaptiveEvolutionIndexComponent::UNeuroadaptiveEvolutionIndexComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuroadaptiveEvolutionIndexComponent::BeginPlay()
{
	Super::BeginPlay();

	NeuroadaptiveEvolutionIndex = CalculateNEI();
	UE_LOG(LogTemp, Log, TEXT("Neuroadaptive Evolution Index (NEI): %0.4f"), NeuroadaptiveEvolutionIndex);
}

float UNeuroadaptiveEvolutionIndexComponent::CalculateNEI()
{
	if (TotalNeurons <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Total number of neurons (N_t) must be greater than zero."));
		NeuroadaptiveEvolutionIndex = 0.0f;
		return 0.0f;
	}

	const float GrowthRatio = NeuronGrowth / TotalNeurons;
	NeuroadaptiveEvolutionIndex = Eta * GrowthRatio;
	return NeuroadaptiveEvolutionIndex;
}
