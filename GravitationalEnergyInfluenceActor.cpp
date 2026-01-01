#include "GravitationalEnergyInfluenceActor.h"

#include "Math/UnrealMathUtility.h"

AGravitationalEnergyInfluenceActor::AGravitationalEnergyInfluenceActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGravitationalEnergyInfluenceActor::BeginPlay()
{
	Super::BeginPlay();
	LastForce = CalculateGravitationalForce(Mass1, Mass2, Distance);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Gravitational Force: %.2e N"), LastForce);
	}
}

double AGravitationalEnergyInfluenceActor::CalculateGravitationalForce(double Mass1Value, double Mass2Value, double DistanceValue) const
{
	if (DistanceValue <= 0.0)
	{
		UE_LOG(LogTemp, Error, TEXT("Distance must be greater than zero."));
		return 0.0;
	}
	return (GravitationalConstant * Mass1Value * Mass2Value) / (DistanceValue * DistanceValue);
}

double AGravitationalEnergyInfluenceActor::GetLastForce() const
{
	return LastForce;
}
