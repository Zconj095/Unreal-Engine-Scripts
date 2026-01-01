#include "GenomeMappingCapacityActor.h"

#include "Math/UnrealMathUtility.h"

AGenomeMappingCapacityActor::AGenomeMappingCapacityActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGenomeMappingCapacityActor::BeginPlay()
{
	Super::BeginPlay();

	GenomeMappingCapacity = CalculateGenomeMappingCapacity(MemoryEfficiencyFactor, NumberOfGenesStored, PowerConstant);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Genome Mapping Capacity (M_g): %.4f"), GenomeMappingCapacity);
	}
}

float AGenomeMappingCapacityActor::CalculateGenomeMappingCapacity(float Lambda, float Ng, float Power) const
{
	if (Ng <= 0.0f || Power <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid parameters: Ng and power constant must be greater than zero."));
		return 0.0f;
	}

	return Lambda * FMath::Pow(Ng, Power);
}

float AGenomeMappingCapacityActor::GetMappingCapacity() const
{
	return GenomeMappingCapacity;
}
