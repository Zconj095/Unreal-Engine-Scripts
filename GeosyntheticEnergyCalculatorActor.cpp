#include "GeosyntheticEnergyCalculatorActor.h"

#include "Math/UnrealMathUtility.h"

AGeosyntheticEnergyCalculatorActor::AGeosyntheticEnergyCalculatorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeosyntheticEnergyCalculatorActor::BeginPlay()
{
	Super::BeginPlay();

	Energy = CalculateGeosyntheticEnergy();
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Geosynthetic Energy (E_g): %.4f"), Energy);
	}
}

float AGeosyntheticEnergyCalculatorActor::GetEnergy() const
{
	return Energy;
}

float AGeosyntheticEnergyCalculatorActor::EnergyFlux(float TimeValue) const
{
	return FMath::Sin(TimeValue) + 2.0f;
}

float AGeosyntheticEnergyCalculatorActor::CalculateGeosyntheticEnergy() const
{
	if (Steps <= 0 || EndTime <= StartTime)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid time range or step count for geosynthetic energy."));
		return 0.0f;
	}

	const float DeltaT = (EndTime - StartTime) / Steps;
	float EnergySum = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float CurrentTime = StartTime + Index * DeltaT;
		const float NextTime = CurrentTime + DeltaT;
		const float Midpoint = 0.5f * (CurrentTime + NextTime);
		const float Flux = EnergyFlux(Midpoint);
		EnergySum += DeltaGg * Flux * DeltaT;
	}

	return EnergySum;
}
