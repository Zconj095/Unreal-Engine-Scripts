#include "GeneticMemoryStorageActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticMemoryStorageActor::AGeneticMemoryStorageActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticMemoryStorageActor::BeginPlay()
{
	Super::BeginPlay();

	GeneticMemoryCapacity = CalculateGeneticMemoryCapacity(NumberOfGenePatterns, MemoryEfficiencyConstant);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Genetic Memory Capacity (M_g): %.4f"), GeneticMemoryCapacity);
	}
}

float AGeneticMemoryStorageActor::CalculateGeneticMemoryCapacity(float Ng, float Lambda) const
{
	if (Ng <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Number of gene patterns must be greater than zero."));
		return 0.0f;
	}

	return Lambda * Ng * FMath::Loge(Ng);
}

float AGeneticMemoryStorageActor::GetMemoryCapacity() const
{
	return GeneticMemoryCapacity;
}
