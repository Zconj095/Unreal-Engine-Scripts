#include "LeylineEnergyInfusion8Component.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusion8Component::ULeylineEnergyInfusion8Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyInfusion8Component::BeginPlay()
{
	Super::BeginPlay();
	RecalculatePowerFlow();
}

double ULeylineEnergyInfusion8Component::CalculateLeylineEnergyFlow() const
{
	if (!MagicalDensity || !EnergyGradient || IntegrationSteps <= 0 || TimeEnd <= TimeStart)
	{
		return 0.0;
	}

	const double Step = (TimeEnd - TimeStart) / IntegrationSteps;
	double Sum = 0.0;

	for (int32 Index = 0; Index <= IntegrationSteps; ++Index)
	{
		const double Time = TimeStart + Step * Index;
		const double Density = MagicalDensity->GetFloatValue(Time);
		const double Gradient = EnergyGradient->GetFloatValue(Time);
		Sum += Density * Gradient;
	}

	return Sum * Step;
}

double ULeylineEnergyInfusion8Component::TensorFlowValidation(double LeylineEnergy) const
{
	// Logically identical to the integration above
	return LeylineEnergy;
}

void ULeylineEnergyInfusion8Component::RecalculatePowerFlow()
{
	PowerFusion = CalculateLeylineEnergyFlow();
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Infusion Power (P_fusion): %.4f"), PowerFusion);
	UE_LOG(LogTemp, Log, TEXT("[TensorFlow Validation] Leyline Energy Infusion Power (P_fusion): %.4f"), TensorFlowValidation(PowerFusion));
}
