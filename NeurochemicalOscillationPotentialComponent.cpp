#include "NeurochemicalOscillationPotentialComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurochemicalOscillationPotentialComponent::UNeurochemicalOscillationPotentialComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeurochemicalOscillationPotentialComponent::BeginPlay()
{
	Super::BeginPlay();
	NeurochemicalPotential = InitialFlux;
}

void UNeurochemicalOscillationPotentialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeAccumulator += DeltaTime;
	NeurochemicalPotential = InitialFlux * FMath::Exp(-DampingCoefficient * TimeAccumulator) * FMath::Cos(Frequency * TimeAccumulator);
	UE_LOG(LogTemp, Log, TEXT("Neurochemical Oscillation Potential (Φₙ(t)): %0.4f"), NeurochemicalPotential);
}

void UNeurochemicalOscillationPotentialComponent::ResetTime()
{
	TimeAccumulator = 0.0f;
}
