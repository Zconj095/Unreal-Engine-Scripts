#include "GeneActivationEnergyActor.h"

#include "Math/UnrealMathUtility.h"

AGeneActivationEnergyActor::AGeneActivationEnergyActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneActivationEnergyActor::BeginPlay()
{
	Super::BeginPlay();
	ActivationEnergy = CalculateActivationEnergy();
	UE_LOG(LogTemp, Display, TEXT("Gene Activation Energy: %.4f"), ActivationEnergy);
}

double AGeneActivationEnergyActor::CalculateActivationEnergy() const
{
	return BaselineEnergy + MagicalEnergyContribution;
}

void AGeneActivationEnergyActor::UpdateMagicalEnergyContribution(double NewContribution)
{
	MagicalEnergyContribution = FMath::Max(0.0, NewContribution);
	ActivationEnergy = CalculateActivationEnergy();
	UE_LOG(LogTemp, Display, TEXT("Updated Gene Activation Energy: %.4f"), ActivationEnergy);
}

double AGeneActivationEnergyActor::GetActivationEnergy() const
{
	return ActivationEnergy;
}
