#include "GeneticMutationProcessingActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticMutationProcessingActor::AGeneticMutationProcessingActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticMutationProcessingActor::BeginPlay()
{
	Super::BeginPlay();

	RateOfMutationAnalysis = CalculateRateOfMutationAnalysis(MutationAnalysisConstant, GeneticPathwaysStudied, MaximumCapacity);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Rate of Genetic Mutation Analysis (ΔG): %.4f"), RateOfMutationAnalysis);
	}
}

float AGeneticMutationProcessingActor::CalculateRateOfMutationAnalysis(float Mu, float Ng, float K) const
{
	if (Ng < 0.0f || K <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid GMP parameters: Ng must be >= 0 and K must be > 0."));
		return 0.0f;
	}

	return Mu * Ng * (1.0f - (Ng / K));
}
