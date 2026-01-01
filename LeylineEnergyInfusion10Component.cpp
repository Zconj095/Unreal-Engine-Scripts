#include "LeylineEnergyInfusion10Component.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusion10Component::ULeylineEnergyInfusion10Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyInfusion10Component::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLeylinePower();
}

double ULeylineEnergyInfusion10Component::EnergyDensity(double Time) const
{
	return FMath::Sin(Time);
}

double ULeylineEnergyInfusion10Component::EnergyGradient(double Time) const
{
	return FMath::Cos(Time);
}

double ULeylineEnergyInfusion10Component::CalculateLeylineEnergyFlow() const
{
	if (IntegrationSteps <= 0 || EndTime <= StartTime)
	{
		return 0.0;
	}

	const double StepSize = (EndTime - StartTime) / IntegrationSteps;
	double Sum = 0.0;
	for (int32 Index = 0; Index <= IntegrationSteps; ++Index)
	{
		const double Time = StartTime + Index * StepSize;
		Sum += EnergyDensity(Time) * EnergyGradient(Time);
	}

	return Sum * StepSize;
}

void ULeylineEnergyInfusion10Component::RecalculateLeylinePower()
{
	PowerThermal = CalculateLeylineEnergyFlow();
	UE_LOG(LogTemp, Log, TEXT("Power Delivered to the Thermal System (P_thermal): %.4f"), PowerThermal);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Calculated Power (P_thermal): %.4f"), PowerThermal);
}
