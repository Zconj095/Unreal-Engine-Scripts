#include "NeurochemicalDimensionalExpansionComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurochemicalDimensionalExpansionComponent::UNeurochemicalDimensionalExpansionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurochemicalDimensionalExpansionComponent::BeginPlay()
{
	Super::BeginPlay();

	NeurochemicalDimensionalExpansionValue = CalculateNDE();
	UE_LOG(LogTemp, Log, TEXT("Neurochemical Dimensional Expansion (NDE): %0.4f"), NeurochemicalDimensionalExpansionValue);
}

float UNeurochemicalDimensionalExpansionComponent::CalculateNDE() const
{
	if (TotalNeurons <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Total neurons (N) must be greater than zero!"));
		return 0.0f;
	}

	const float LogTerm = FMath::Log2(1.0f + ConnectivityDensity);
	return Lambda * FMath::Pow(TotalNeurons, Dimensionality) * LogTerm;
}
