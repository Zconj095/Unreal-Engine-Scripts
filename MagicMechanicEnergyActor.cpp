#include "MagicMechanicEnergyActor.h"

AMagicMechanicEnergyActor::AMagicMechanicEnergyActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMagicMechanicEnergyActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeTotalEnergy();
}

void AMagicMechanicEnergyActor::ComputeTotalEnergy() const
{
	if (MagicalContributions.Num() != MechanicalContributions.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Contribution arrays must match length."));
		return;
	}

	float TotalEnergy = 0.f;
	for (int32 Index = 0; Index < MagicalContributions.Num(); ++Index)
	{
		TotalEnergy += MagicalContributions[Index] + MechanicalContributions[Index];
	}

	UE_LOG(LogTemp, Log, TEXT("Total System Energy (E_total): %f"), TotalEnergy);
}
