#include "GeneticCognitiveLoadActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticCognitiveLoadActor::AGeneticCognitiveLoadActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticCognitiveLoadActor::BeginPlay()
{
	Super::BeginPlay();

	if (EndTime <= StartTime)
	{
		UE_LOG(LogTemp, Error, TEXT("EndTime must be greater than StartTime."));
		return;
	}

	GeneticCognitiveLoad = ComputeGCL(EnvironmentalStimuli, GeneticStimuli, TimeConstant, StartTime, EndTime, IntegrationSteps);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Genetic Cognitive Load (L_g): %.4f"), GeneticCognitiveLoad);
	}
}

float AGeneticCognitiveLoadActor::ComputeGCL(float E, float G, float Tau, float T0, float T, int32 Steps) const
{
	if (Tau <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Time constant must be greater than zero."));
		return 0.0f;
	}

	if (Steps <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("IntegrationSteps must be positive."));
		return 0.0f;
	}

	const float Dt = (T - T0) / Steps;
	float Accumulator = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float CurrentTime = T0 + Index * Dt;
		const float Interaction = EvaluateInteraction(E, G, CurrentTime);
		Accumulator += Interaction * Dt;
	}

	return Accumulator / Tau;
}

float AGeneticCognitiveLoadActor::EvaluateInteraction(float E, float G, float Time) const
{
	return E * FMath::Sin(Time) + G * FMath::Cos(Time);
}
