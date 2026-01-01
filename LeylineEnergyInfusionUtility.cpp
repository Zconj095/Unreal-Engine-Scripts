#include "LeylineEnergyInfusionUtility.h"
#include "Engine/Engine.h"

double ULeylineEnergyInfusionUtility::CalculateEfficiency(double OutputEnergy, double InputEnergy)
{
	if (FMath::IsNearlyZero(InputEnergy))
	{
		UE_LOG(LogTemp, Error, TEXT("Energy input cannot be zero."));
		return 0.0;
	}

	return OutputEnergy / InputEnergy;
}

void ULeylineEnergyInfusionUtility::Test()
{
	const double P_out = 100.0;
	const double P_in = 200.0;
	const double Efficiency = CalculateEfficiency(P_out, P_in);
	UE_LOG(LogTemp, Log, TEXT("Energy Transfer Efficiency: %.2f"), Efficiency);
}
