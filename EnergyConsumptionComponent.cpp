#include "EnergyConsumptionComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyConsumptionComponent::UEnergyConsumptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	BaselineEnergy = 10.0f;
	SeasonalEnergy = 5.0f;
	AngularFrequency = 2.0f * PI / 365.0f;
	ObservationPeriod = 10.0f;
	Resolution = 100;
}

void UEnergyConsumptionComponent::BeginPlay()
{
	Super::BeginPlay();

	const float TotalEnergy = CalculateEnergyConsumption();
	UE_LOG(LogTemp, Log, TEXT("Total Energy Consumption (Eb): %f"), TotalEnergy);
}

float UEnergyConsumptionComponent::CalculateEnergyConsumption() const
{
	if (Resolution <= 0 || ObservationPeriod <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid resolution or observation period for energy consumption."));
		return 0.0f;
	}

	const float Dt = ObservationPeriod / static_cast<float>(Resolution);
	float TotalEnergy = 0.0f;

	for (int32 Index = 0; Index < Resolution; ++Index)
	{
		const float T1 = Index * Dt;
		const float T2 = (Index + 1) * Dt;
		const float EnergyAtT1 = EnergyFunction(T1);
		const float EnergyAtT2 = EnergyFunction(T2);
		TotalEnergy += 0.5f * (EnergyAtT1 + EnergyAtT2) * Dt;
	}

	return TotalEnergy;
}

float UEnergyConsumptionComponent::EnergyFunction(float Time) const
{
	return BaselineEnergy + SeasonalEnergy * FMath::Cos(AngularFrequency * Time);
}
