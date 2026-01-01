#include "GeosyntheticEnergyIntegrationActor.h"

#include "Math/UnrealMathUtility.h"

AGeosyntheticEnergyIntegrationActor::AGeosyntheticEnergyIntegrationActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeosyntheticEnergyIntegrationActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentTime += DeltaSeconds;
	const float EnergyFlux = GeologicalEnergyFlux(CurrentTime);
	const float EnergyContribution = FreeEnergyChange * EnergyFlux;
	IntegralSum += EnergyContribution * TimeStep;
	IntegratedEnergy = IntegralSum;

	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Time: %.2f, Energy Flux: %.4f, Integrated Energy: %.4f"), CurrentTime, EnergyFlux, IntegratedEnergy);
	}
}

float AGeosyntheticEnergyIntegrationActor::GetIntegratedEnergy() const
{
	return IntegratedEnergy;
}

float AGeosyntheticEnergyIntegrationActor::GeologicalEnergyFlux(float TimeValue) const
{
	return FMath::Sin(TimeValue) + 1.0f;
}
