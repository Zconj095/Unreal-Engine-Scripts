#include "GeochemicalOscillationPotentialActor.h"

#include "Math/UnrealMathUtility.h"

AGeochemicalOscillationPotentialActor::AGeochemicalOscillationPotentialActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeochemicalOscillationPotentialActor::BeginPlay()
{
	Super::BeginPlay();
	AccumulatedTime = 0.0f;
}

void AGeochemicalOscillationPotentialActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AccumulatedTime += DeltaSeconds * TimeStepScale;
	const float OscillationValue = CalculateOscillationPotential(AccumulatedTime);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Time: %.2f, Geochemical Oscillation Potential (Φg): %.2f"), AccumulatedTime, OscillationValue);
	}
}

float AGeochemicalOscillationPotentialActor::CalculateOscillationPotential(float TimeSeconds) const
{
	return InitialFlux * FMath::Exp(-DampingCoefficient * TimeSeconds) * FMath::Cos(OscillationFrequency * TimeSeconds);
}
