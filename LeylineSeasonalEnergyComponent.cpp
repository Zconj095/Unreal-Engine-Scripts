#include "LeylineSeasonalEnergyComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineSeasonalEnergyComponent::ULeylineSeasonalEnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineSeasonalEnergyComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateSeasonalEnergy();
}

double ULeylineSeasonalEnergyComponent::Phi(double Time) const
{
	return FMath::Sin(2.0 * PI * Time);
}

double ULeylineSeasonalEnergyComponent::GradientPsi(double Time) const
{
	return FMath::Cos(2.0 * PI * Time);
}

double ULeylineSeasonalEnergyComponent::IntegrateSeasonalEnergy() const
{
	if (IntegrationSteps <= 0 || EndTime <= StartTime)
	{
		return 0.0;
	}

	const double Step = (EndTime - StartTime) / IntegrationSteps;
	double Sum = 0.0;

	for (int32 Index = 0; Index <= IntegrationSteps; ++Index)
	{
		const double Time = StartTime + Index * Step;
		Sum += Phi(Time) * GradientPsi(Time);
	}

	return Sum * Step;
}

void ULeylineSeasonalEnergyComponent::LogResult(double Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Leyline Seasonal Energy Flow (P_seasonal): %.4f"), Value);
}

void ULeylineSeasonalEnergyComponent::RecalculateSeasonalEnergy()
{
	SeasonalEnergy = IntegrateSeasonalEnergy();
	LogResult(SeasonalEnergy);
}
