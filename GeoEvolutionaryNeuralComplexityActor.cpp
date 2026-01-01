#include "GeoEvolutionaryNeuralComplexityActor.h"

#include "Math/UnrealMathUtility.h"

AGeoEvolutionaryNeuralComplexityActor::AGeoEvolutionaryNeuralComplexityActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeoEvolutionaryNeuralComplexityActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ElapsedTime += DeltaSeconds;
	CurrentComplexity = CalculateGNC(InitialComplexity, GrowthRate, ElapsedTime);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Time: %.2f, Complexity: %.4f"), ElapsedTime, CurrentComplexity);
	}
}

float AGeoEvolutionaryNeuralComplexityActor::GetCurrentComplexity() const
{
	return CurrentComplexity;
}

float AGeoEvolutionaryNeuralComplexityActor::CalculateGNC(float C0, float RG, float TimeSeconds) const
{
	return C0 * FMath::Exp(RG * TimeSeconds);
}
