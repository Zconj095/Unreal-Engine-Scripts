#include "GeneticNeuralComplexityActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticNeuralComplexityActor::AGeneticNeuralComplexityActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticNeuralComplexityActor::BeginPlay()
{
	Super::BeginPlay();

	GeneticNeuralComplexity = CalculateGeneticNeuralComplexity(NumberOfGeneticPatterns, NeuralConnectivityConstant, GeneticEncodingEfficiency);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Genetic Neural Complexity (C_g): %.4f"), GeneticNeuralComplexity);
	}
}

float AGeneticNeuralComplexityActor::CalculateGeneticNeuralComplexity(float Ng, float Alpha, float Beta) const
{
	if (Ng <= 0.0f || Beta < 0.0f || Beta > 1.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid parameters: Ng must be > 0 and β must be within [0,1]."));
		return 0.0f;
	}

	return Alpha * Ng * FMath::Loge(Ng) * Beta;
}

float AGeneticNeuralComplexityActor::GetNeuralComplexity() const
{
	return GeneticNeuralComplexity;
}
