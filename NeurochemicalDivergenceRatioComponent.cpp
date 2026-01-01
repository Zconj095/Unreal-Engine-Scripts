#include "NeurochemicalDivergenceRatioComponent.h"
#include "Logging/LogMacros.h"

UNeurochemicalDivergenceRatioComponent::UNeurochemicalDivergenceRatioComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurochemicalDivergenceRatioComponent::BeginPlay()
{
	Super::BeginPlay();

	NeurochemicalDivergenceRatio = CalculateNDR();
	UE_LOG(LogTemp, Log, TEXT("Neurochemical Divergence Ratio (NDR): %0.4f"), NeurochemicalDivergenceRatio);
}

float UNeurochemicalDivergenceRatioComponent::CalculateNDR() const
{
	if (FMath::IsNearlyZero(Omega))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Total cognitive energy (Omega) cannot be zero!"));
		return 0.0f;
	}

	return (DeltaR / Omega) * Lambda;
}
