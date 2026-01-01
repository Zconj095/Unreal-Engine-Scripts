#include "EnergyAbsorptionStorageComponent.h"

UEnergyAbsorptionStorageComponent::UEnergyAbsorptionStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnergyAbsorptionStorageComponent::BeginPlay()
{
	Super::BeginPlay();
	LogExampleEfficiency();
}

double UEnergyAbsorptionStorageComponent::CalculateEfficiency(double EnergyStored, double EnergyInput)
{
	if (FMath::IsNearlyZero(EnergyInput))
	{
		UE_LOG(LogTemp, Warning, TEXT("Energy input cannot be zero."));
		return 0.0;
	}

	return EnergyStored / EnergyInput;
}

void UEnergyAbsorptionStorageComponent::LogExampleEfficiency() const
{
	const double Efficiency = CalculateEfficiency(1e6, 2e6);
	UE_LOG(LogTemp, Log, TEXT("Energy Storage Efficiency (η): %.2f%%"), Efficiency * 100.0);
}
