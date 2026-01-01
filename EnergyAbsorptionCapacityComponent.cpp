#include "EnergyAbsorptionCapacityComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyAbsorptionCapacityComponent::UEnergyAbsorptionCapacityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Alpha = 0.5f;
	IntegrationTimeStep = 0.1f;
	bSimulate = true;
	Time = 0.0f;
	Ae = 0.0f;
	AccumulatedEnergy = 0.0f;
	LastPower = 0.0f;
	bHasLastPower = false;
}

void UEnergyAbsorptionCapacityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnergyAbsorptionCapacityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bSimulate)
	{
		return;
	}

	Time += DeltaTime;
	const float Power = CalculatePower(Time);

	if (bHasLastPower)
	{
		AccumulatedEnergy += (LastPower + Power) * IntegrationTimeStep * 0.5f;
	}
	else
	{
		bHasLastPower = true;
	}

	LastPower = Power;
	Ae = Alpha * AccumulatedEnergy;
}

float UEnergyAbsorptionCapacityComponent::CalculatePower(float CurrentTime) const
{
	return FMath::Sin(CurrentTime) + 1.0f;
}
