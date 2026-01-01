#include "QuantumFusionProbabilityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UQuantumFusionProbabilityComponent::UQuantumFusionProbabilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumFusionProbabilityComponent::BeginPlay()
{
	Super::BeginPlay();

	FusionProbability = CalculateFusionProbability();
	UE_LOG(LogTemp, Log, TEXT("Fusion Probability: %0.6f"), FusionProbability);
}

float UQuantumFusionProbabilityComponent::CalculateFusionProbability() const
{
	if (Temperature <= 0.0f || BoltzmannConstant <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid temperature or Boltzmann constant."));
		return 0.0f;
	}

	const double Exponent = -ActivationEnergy / (BoltzmannConstant * Temperature);
	return static_cast<float>(FMath::Exp(Exponent) * MagicalEnergy);
}
