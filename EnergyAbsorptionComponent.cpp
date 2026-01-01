#include "EnergyAbsorptionComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyAbsorptionComponent::UEnergyAbsorptionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Efficiency = 0.8f;
	ObservationPeriod = 10.0f;
	IntegrationSteps = 100;
}

void UEnergyAbsorptionComponent::BeginPlay()
{
	Super::BeginPlay();

	const float EnergyAbsorption = CalculateEnergyAbsorption(Efficiency, ObservationPeriod, IntegrationSteps);
	UE_LOG(LogTemp, Log, TEXT("Energy Absorption (Ea): %f"), EnergyAbsorption);
}

float UEnergyAbsorptionComponent::PhiA(float Time) const
{
	return FMath::Sin(Time) + 1.0f;
}

float UEnergyAbsorptionComponent::CalculateEnergyAbsorption(float Eta, float Period, int32 Steps) const
{
	if (Steps <= 0 || Period <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for energy absorption integration."));
		return 0.0f;
	}

	const float Dt = Period / static_cast<float>(Steps);
	float Energy = 0.0f;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const float T1 = Index * Dt;
		const float T2 = (Index + 1) * Dt;
		const float Flux1 = PhiA(T1);
		const float Flux2 = PhiA(T2);
		Energy += 0.5f * (Flux1 + Flux2) * Dt;
	}

	return Eta * Energy;
}
