#include "OscillationPotentialComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UOscillationPotentialComponent::UOscillationPotentialComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOscillationPotentialComponent::BeginPlay()
{
	Super::BeginPlay();
	OscillationPotential = CalculateOscillationPotential(0.0f);
}

void UOscillationPotentialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	OscillationPotential = CalculateOscillationPotential(CurrentTime);
	UE_LOG(LogTemp, Log, TEXT("Oscillation Potential (Φn): %0.2f"), OscillationPotential);
}

float UOscillationPotentialComponent::CalculateOscillationPotential(float Time) const
{
	const float DampingFactor = FMath::Exp(-DampingCoefficient * Time);
	const float Oscillation = FMath::Cos(2.0f * PI * OscillationFrequency * Time);
	return InitialEnergyFlux * DampingFactor * Oscillation;
}
