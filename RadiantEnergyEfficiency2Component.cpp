#include "RadiantEnergyEfficiency2Component.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URadiantEnergyEfficiency2Component::URadiantEnergyEfficiency2Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URadiantEnergyEfficiency2Component::BeginPlay()
{
	Super::BeginPlay();

	Efficiency = CalculateRadiantEnergyEfficiency();
	UE_LOG(LogTemp, Log, TEXT("Radiant Energy Efficiency: %0.4f"), Efficiency);
}

float URadiantEnergyEfficiency2Component::CalculateRadiantEnergyEfficiency() const
{
	const float Denominator = PowerInput + MagicalContribution;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid denominator for radiant efficiency."));
		return 0.0f;
	}

	return PowerOutput / Denominator;
}
