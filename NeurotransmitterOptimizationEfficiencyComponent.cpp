#include "NeurotransmitterOptimizationEfficiencyComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurotransmitterOptimizationEfficiencyComponent::UNeurotransmitterOptimizationEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurotransmitterOptimizationEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	NeurotransmitterOptimizationEfficiency = CalculateNOE();
	if (NeurotransmitterOptimizationEfficiency > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Neurotransmitter Optimization Efficiency (NOE): %0.4f"), NeurotransmitterOptimizationEfficiency);
	}
}

float UNeurotransmitterOptimizationEfficiencyComponent::CalculateNOE() const
{
	if (FMath::IsNearlyZero(OptimizationTime) || FMath::IsNearlyZero(CognitiveEnergyExpenditure))
	{
		UE_LOG(LogTemp, Error, TEXT("Optimization time (T) and cognitive energy expenditure (mu) cannot be zero!"));
		return 0.0f;
	}

	return Gamma * (NeuralActivity / (OptimizationTime * CognitiveEnergyExpenditure));
}
