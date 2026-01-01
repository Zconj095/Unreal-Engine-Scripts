#include "EnergyFlowOptimizationComponent2.h"
#include "Math/UnrealMathUtility.h"

UEnergyFlowOptimizationComponent2::UEnergyFlowOptimizationComponent2()
{
	PrimaryComponentTick.bCanEverTick = false;
	TotalEnergy = 100.0;
	Resistances = {2.0, 3.0, 5.0};
}

void UEnergyFlowOptimizationComponent2::BeginPlay()
{
	Super::BeginPlay();

	const double Power = CalculatePower(TotalEnergy, Resistances);
	UE_LOG(LogTemp, Log, TEXT("Power allocated: %f"), Power);
}

double UEnergyFlowOptimizationComponent2::CalculatePower(double Energy, const TArray<double>& ResistanceArray) const
{
	if (ResistanceArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Resistance array cannot be empty."));
		return 0.0;
	}

	double InverseSum = 0.0;
	for (double Resistance : ResistanceArray)
	{
		if (Resistance <= 0.0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Resistance values must be greater than zero."));
			return 0.0;
		}
		InverseSum += 1.0 / Resistance;
	}

	if (FMath::IsNearlyZero(InverseSum))
	{
		UE_LOG(LogTemp, Warning, TEXT("Inverse resistance sum cannot be zero."));
		return 0.0;
	}

	return Energy / InverseSum;
}
