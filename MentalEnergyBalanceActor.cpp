#include "MentalEnergyBalanceActor.h"

AMentalEnergyBalanceActor::AMentalEnergyBalanceActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMentalEnergyBalanceActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeMentalEnergy();
}

void AMentalEnergyBalanceActor::ComputeMentalEnergy() const
{
	if (CognitiveCapacities.Num() != Resistances.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Array lengths must match."));
		return;
	}

	float TotalEnergy = 0.f;
	for (int32 Index = 0; Index < CognitiveCapacities.Num(); ++Index)
	{
		if (Resistances[Index] == 0.f) continue;
		TotalEnergy += CognitiveCapacities[Index] / Resistances[Index];
	}

	TotalEnergy += MagicalCoefficient * MagicalFlux;

	UE_LOG(LogTemp, Log, TEXT("Total Mental Energy (E_mental): %.4f"), TotalEnergy);
}
