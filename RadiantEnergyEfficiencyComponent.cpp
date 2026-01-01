#include "RadiantEnergyEfficiencyComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URadiantEnergyEfficiencyComponent::URadiantEnergyEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URadiantEnergyEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	Efficiency = CalculateEfficiency();
	UE_LOG(LogTemp, Log, TEXT("Radiant Energy Efficiency (η): %0.4f"), Efficiency);
}

float URadiantEnergyEfficiencyComponent::CalculateEfficiency() const
{
	const float Denominator = EnergyInput + MagicalEnhancement * EnergyGradient;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid denominator for efficiency calculation."));
		return 0.0f;
	}

	return EnergyOutput / Denominator;
}
