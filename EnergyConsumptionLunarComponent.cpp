#include "EnergyConsumptionLunarComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyConsumptionLunarComponent::UEnergyConsumptionLunarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BaselineEnergy = 10.0f;
	LunarEnergy = 5.0f;
	AngularFrequency = 1.0f;
	ComputationTime = 10.0f;
	IntegrationSteps = 1000;
	EnergyConsumption = 0.0f;
}

void UEnergyConsumptionLunarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	EnergyConsumption = CalculateEnergyConsumption(BaselineEnergy, LunarEnergy, AngularFrequency, ComputationTime, IntegrationSteps);
	UE_LOG(LogTemp, Log, TEXT("Lunar Energy Consumption (E_l): %f"), EnergyConsumption);
}

float UEnergyConsumptionLunarComponent::CalculateEnergyConsumption(float Pb, float Pl, float OmegaM, float Duration, int32 Steps) const
{
	if (Duration <= 0.0f || Steps <= 0)
	{
		return 0.0f;
	}

	const float DeltaT = Duration / static_cast<float>(Steps);
	float TotalEnergy = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float T = Index * DeltaT;
		const float EnergyAtT = Pb + Pl * FMath::Cos(OmegaM * T);
		TotalEnergy += EnergyAtT * DeltaT;
	}

	return TotalEnergy;
}
