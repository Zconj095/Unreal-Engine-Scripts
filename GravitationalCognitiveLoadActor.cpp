#include "GravitationalCognitiveLoadActor.h"

#include "Math/UnrealMathUtility.h"

AGravitationalCognitiveLoadActor::AGravitationalCognitiveLoadActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGravitationalCognitiveLoadActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CognitiveLoad = CalculateGCL(MoonMass, EffectiveMass, Distance);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Gravitational Cognitive Load (Lg): %.6e N"), CognitiveLoad);
	}
}

float AGravitationalCognitiveLoadActor::GetCognitiveLoad() const
{
	return CognitiveLoad;
}

float AGravitationalCognitiveLoadActor::CalculateGCL(float Mm, float Me, float R) const
{
	if (R <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Distance must be greater than zero."));
		return 0.0f;
	}
	return (GravitationalConstant * Mm * Me) / (R * R);
}
