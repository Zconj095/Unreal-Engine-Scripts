#include "GeoEvolutionaryCognitiveLoadActor.h"

#include "Math/UnrealMathUtility.h"

AGeoEvolutionaryCognitiveLoadActor::AGeoEvolutionaryCognitiveLoadActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeoEvolutionaryCognitiveLoadActor::BeginPlay()
{
	Super::BeginPlay();
	CumulativeSum = 0.0f;
	CognitiveLoad = 0.0f;
}

void AGeoEvolutionaryCognitiveLoadActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCognitiveLoad(DeltaSeconds);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Interaction Value: %.4f, Cognitive Load: %.4f"), InteractionFunction(EnvironmentalStimuli, GeneticData), CognitiveLoad);
	}
}

void AGeoEvolutionaryCognitiveLoadActor::UpdateCognitiveLoad(float DeltaTime)
{
	const float Interaction = InteractionFunction(EnvironmentalStimuli, GeneticData);
	CumulativeSum += Interaction * DeltaTime;
	if (TimeConstant <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("TimeConstant must be greater than zero."));
		CognitiveLoad = 0.0f;
		return;
	}
	CognitiveLoad = CumulativeSum / TimeConstant;
}

float AGeoEvolutionaryCognitiveLoadActor::InteractionFunction(float Environmental, float Genetic) const
{
	return Environmental + Genetic;
}
