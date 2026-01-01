#include "PatternFormationPotentialComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPatternFormationPotentialComponent::UPatternFormationPotentialComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPatternFormationPotentialComponent::BeginPlay()
{
	Super::BeginPlay();
	PreviousTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
}

void UPatternFormationPotentialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	const float Delta = CurrentTime - PreviousTime;

	const float CurrentIntegrand = EvaluateIntegrand(CurrentTime);
	const float PreviousIntegrand = EvaluateIntegrand(PreviousTime);

	PatternFormationPotential += (CurrentIntegrand + PreviousIntegrand) * 0.5f * Delta;
	PreviousTime = CurrentTime;

	UE_LOG(LogTemp, Log, TEXT("Pattern Formation Potential (Pf): %0.2f"), PatternFormationPotential);
}

float UPatternFormationPotentialComponent::EvaluateIntegrand(float Time) const
{
	const float WaveFunction = WaveAmplitude * FMath::Sin(2.0f * PI * WaveFrequency * Time);
	const float HamiltonianValue = HamiltonianScale * WaveFunction;
	return WaveFunction * HamiltonianValue;
}
