#include "QuantumProbabilityDensityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UQuantumProbabilityDensityComponent::UQuantumProbabilityDensityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumProbabilityDensityComponent::BeginPlay()
{
	Super::BeginPlay();

	const FProbabilityQuantumComplex Wavefunction = CalculateWavefunction(Position, TimeParameter);
	ProbabilityDensity = Wavefunction.MagnitudeSquared();
	UE_LOG(LogTemp, Log, TEXT("Probability Density P(%0.2f, %0.2f) = %0.4f"), Position, TimeParameter, ProbabilityDensity);
}

FProbabilityQuantumComplex UQuantumProbabilityDensityComponent::CalculateWavefunction(float X, float T) const
{
	const float Phase = Wavevector * X - AngularFrequency * T;
	const float Real = FMath::Cos(Phase);
	const float Imag = FMath::Sin(Phase);
	return FProbabilityQuantumComplex{Amplitude * Real, Amplitude * Imag};
}
