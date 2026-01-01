#include "EnergyConsumptionComponent2.h"
#include "Math/UnrealMathUtility.h"

UEnergyConsumptionComponent2::UEnergyConsumptionComponent2()
{
	PrimaryComponentTick.bCanEverTick = false;
	BaselineEnergy = 10.0f;
	CelestialEnergy = 5.0f;
	AngularFrequency = 2.0f;
	ObservationPeriod = 10.0f;
	IntegrationSteps = 1000;
}

void UEnergyConsumptionComponent2::BeginPlay()
{
	Super::BeginPlay();

	const float TotalEnergy = CalculateEnergyConsumption(BaselineEnergy, CelestialEnergy, AngularFrequency, ObservationPeriod, IntegrationSteps);
	UE_LOG(LogTemp, Log, TEXT("Total Energy Consumption: %f"), TotalEnergy);
}

float UEnergyConsumptionComponent2::CalculateEnergyConsumption(float Pb, float Pc, float Omega, float Duration, int32 Steps) const
{
	if (Steps <= 0 || Duration <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for EnergyConsumptionComponent2."));
		return 0.0f;
	}

	const float StepSize = Duration / static_cast<float>(Steps);
	float TotalEnergy = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float T = Index * StepSize;
		const float EnergyAtT = Pb + Pc * FMath::Cos(Omega * T);
		TotalEnergy += EnergyAtT * StepSize;
	}

	return TotalEnergy;
}
