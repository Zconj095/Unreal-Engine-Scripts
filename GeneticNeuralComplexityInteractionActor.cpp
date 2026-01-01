#include "GeneticNeuralComplexityInteractionActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticNeuralComplexityInteractionActor::AGeneticNeuralComplexityInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticNeuralComplexityInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	GeneticNeuralComplexity = CalculateGeneticNeuralComplexity(EfficiencyConstant, NeuralPathways, GeneticPatterns);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Genetic-Neural Complexity (C_g): %.4f"), GeneticNeuralComplexity);
	}
}

float AGeneticNeuralComplexityInteractionActor::CalculateGeneticNeuralComplexity(float Beta, float Nn, float Ng) const
{
	if (Ng <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Number of genetic patterns (Ng) must be greater than zero."));
		return 0.0f;
	}

	return Beta * Nn * FMath::Loge(Ng);
}

float AGeneticNeuralComplexityInteractionActor::GetComplexity() const
{
	return GeneticNeuralComplexity;
}
