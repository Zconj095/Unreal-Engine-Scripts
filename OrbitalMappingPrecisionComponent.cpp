#include "OrbitalMappingPrecisionComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UOrbitalMappingPrecisionComponent::UOrbitalMappingPrecisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitalMappingPrecisionComponent::BeginPlay()
{
	Super::BeginPlay();
	Precision = CalculateOrbitalMappingPrecision();
}

void UOrbitalMappingPrecisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Precision = CalculateOrbitalMappingPrecision();
	UE_LOG(LogTemp, Log, TEXT("Orbital Mapping Precision: %0.4f"), Precision);
}

float UOrbitalMappingPrecisionComponent::CalculateOrbitalMappingPrecision() const
{
	const float Exponent = -Sensitivity * (ObservedDensity - ThresholdDensity);
	const float Denominator = 1.0f + FMath::Exp(Exponent);
	return PredictionAccuracy / Denominator;
}
