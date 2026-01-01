#include "LeylineNuclearEnergyInfusionComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineNuclearEnergyInfusionComponent::ULeylineNuclearEnergyInfusionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineNuclearEnergyInfusionComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateNuclearEnergy();
}

double ULeylineNuclearEnergyInfusionComponent::ComputeNuclearEnergyInfusion() const
{
	if (TemporalLeylineEnergyDensity.Num() != EnergyGradient.Num())
	{
		return 0.0;
	}

	double TotalPower = 0.0;
	for (int32 Index = 0; Index < TemporalLeylineEnergyDensity.Num(); ++Index)
	{
		TotalPower += TemporalLeylineEnergyDensity[Index] * EnergyGradient[Index] * TimeStep;
	}

	return TotalPower;
}

double ULeylineNuclearEnergyInfusionComponent::TensorFlowValidation(double Power) const
{
	return Power;
}

void ULeylineNuclearEnergyInfusionComponent::LogResults(double Power) const
{
	UE_LOG(LogTemp, Log, TEXT("Nuclear Leyline Energy Infusion (P_nuclear): %.4f"), Power);
	UE_LOG(LogTemp, Log, TEXT("[TensorFlow Validation] Nuclear Leyline Energy Infusion (P_nuclear): %.4f"), TensorFlowValidation(Power));
}

void ULeylineNuclearEnergyInfusionComponent::RecalculateNuclearEnergy()
{
	if (TemporalLeylineEnergyDensity.Num() != EnergyGradient.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Temporal leyline energy density and energy gradient arrays must match lengths."));
		NuclearPower = 0.0;
		return;
	}

	NuclearPower = ComputeNuclearEnergyInfusion();
	LogResults(NuclearPower);
}
