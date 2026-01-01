#include "OscillationPotentialCalculatorComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UOscillationPotentialCalculatorComponent::UOscillationPotentialCalculatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOscillationPotentialCalculatorComponent::BeginPlay()
{
	Super::BeginPlay();
	OscillationPotential = CalculateOscillationPotential(CurrentTime);
}

void UOscillationPotentialCalculatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime += DeltaTime * TimeStep;
	OscillationPotential = CalculateOscillationPotential(CurrentTime);
	UE_LOG(LogTemp, Log, TEXT("Time: %0.2f, Oscillation Potential (Φ_g): %0.4f"), CurrentTime, OscillationPotential);
}

float UOscillationPotentialCalculatorComponent::CalculateOscillationPotential(float Time) const
{
	const float DampingFactor = FMath::Exp(-DampingCoefficient * Time);
	const float Oscillation = FMath::Cos(2.0f * PI * Frequency * Time);
	return InitialFlux * DampingFactor * Oscillation;
}
