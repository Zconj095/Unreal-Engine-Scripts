#include "LeylineEnergyInfusion7Component.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusion7Component::ULeylineEnergyInfusion7Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyInfusion7Component::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLeylineEnergy();
}

double ULeylineEnergyInfusion7Component::Density(double Time) const
{
	return FMath::Sin(Time);
}

double ULeylineEnergyInfusion7Component::Gradient(double Time) const
{
	return FMath::Cos(Time);
}

double ULeylineEnergyInfusion7Component::CalculateLeylineEnergy() const
{
	if (IntegrationSteps <= 0 || TimeEnd <= TimeStart)
	{
		return 0.0;
	}

	const double StepSize = (TimeEnd - TimeStart) / IntegrationSteps;
	double Sum = 0.0;

	for (int32 Index = 0; Index <= IntegrationSteps; ++Index)
	{
		const double Time = TimeStart + Index * StepSize;
		Sum += Density(Time) * Gradient(Time);
	}

	return Sum * StepSize;
}

double ULeylineEnergyInfusion7Component::TensorFlowValidation() const
{
	return CalculateLeylineEnergy();
}

void ULeylineEnergyInfusion7Component::LogResults() const
{
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Infusion (P_cyber): %.4f"), LeylinePower);
	UE_LOG(LogTemp, Log, TEXT("[TensorFlow Validation] Leyline Energy Infusion (P_cyber): %.4f"), TensorFlowValidation());
}

void ULeylineEnergyInfusion7Component::RecalculateLeylineEnergy()
{
	LeylinePower = CalculateLeylineEnergy();
	LogResults();
}
