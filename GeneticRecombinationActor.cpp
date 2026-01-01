#include "GeneticRecombinationActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticRecombinationActor::AGeneticRecombinationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticRecombinationActor::BeginPlay()
{
	Super::BeginPlay();

	if (bLogOnBeginPlay)
	{
		LogCurrentProbability();
	}
}

double AGeneticRecombinationActor::CalculateCrossoverProbability(double Beta, double Distance) const
{
	return 1.0 - FMath::Exp(-Beta * Distance);
}

void AGeneticRecombinationActor::LogCurrentProbability() const
{
	const double Probability = CalculateCrossoverProbability(RecombinationRate, PhysicalDistance);
	UE_LOG(LogTemp, Display, TEXT("Crossover Probability: %.4f"), Probability);
}
