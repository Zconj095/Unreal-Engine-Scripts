#include "LeylineEnergyIntegration1Component.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyIntegration1Component::ULeylineEnergyIntegration1Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyIntegration1Component::BeginPlay()
{
	Super::BeginPlay();
	RecalculateEnergyFlow();
}

double ULeylineEnergyIntegration1Component::TemporalEnergyDensity(double Time) const
{
	return FMath::Sin(2.0 * PI * Time);
}

double ULeylineEnergyIntegration1Component::EnergyGradient(double Time) const
{
	return FMath::Cos(2.0 * PI * Time);
}

double ULeylineEnergyIntegration1Component::IntegrateEnergyFlow() const
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
		Sum += TemporalEnergyDensity(Time) * EnergyGradient(Time);
	}

	return Sum * StepSize;
}

void ULeylineEnergyIntegration1Component::LogResult(double Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Total Energy Flow (P): %.4f"), Value);
}

void ULeylineEnergyIntegration1Component::RecalculateEnergyFlow()
{
	EnergyFlow = IntegrateEnergyFlow();
	LogResult(EnergyFlow);
}
