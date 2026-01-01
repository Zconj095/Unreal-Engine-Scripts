#include "MutationRateSimulatorActor.h"
#include "Math/UnrealMathUtility.h"

AMutationRateSimulatorActor::AMutationRateSimulatorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMutationRateSimulatorActor::BeginPlay()
{
	Super::BeginPlay();

	Simulate();
}

void AMutationRateSimulatorActor::Simulate()
{
	if (MagicalEnergyTimeline.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No magical energy data provided."));
		return;
	}

	for (int32 Index = 0; Index < MagicalEnergyTimeline.Num(); ++Index)
	{
		const float Energy = MagicalEnergyTimeline[Index];
		const float MutationRate = ComputeMutationRate(Energy);
		UE_LOG(LogTemp, Log, TEXT("Time %d: M(t) = %.4f"), Index, MutationRate);
	}
}

float AMutationRateSimulatorActor::ComputeMutationRate(float MagicalEnergy) const
{
	return BaselineRate + MagicEnhancement * MagicalEnergy;
}
