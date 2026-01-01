#include "GeosyntheticReactionPotentialActor.h"

#include "Math/UnrealMathUtility.h"

AGeosyntheticReactionPotentialActor::AGeosyntheticReactionPotentialActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeosyntheticReactionPotentialActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AccumulatedTime += DeltaSeconds * TimeStepScale;
	ReactionPotential += CalculatePotential(AccumulatedTime) * TimeStepScale;
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Time: %.2f, Geosynthetic Reaction Potential (Pr): %.4f"), AccumulatedTime, ReactionPotential);
	}
}

float AGeosyntheticReactionPotentialActor::GetReactionPotential() const
{
	return ReactionPotential;
}

float AGeosyntheticReactionPotentialActor::CalculatePotential(float TimeValue) const
{
	const float Wavefunction = WavefunctionAmplitude * FMath::Sin(2.0f * PI * TimeValue);
	const float HamiltonianEffect = HamiltonianConstant * Wavefunction;
	return Wavefunction * HamiltonianEffect;
}
