#include "LeylineEnergyInfusion9Component.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusion9Component::ULeylineEnergyInfusion9Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyInfusion9Component::BeginPlay()
{
	Super::BeginPlay();
	EnsureFieldsInitialized();
	RecalculateLeylineEnergy();
}

void ULeylineEnergyInfusion9Component::EnsureFieldsInitialized()
{
	if (TemporalLeylineEnergyDensity.Num() == 0)
	{
		TemporalLeylineEnergyDensity.SetNumZeroed(100);
		for (int32 Index = 0; Index < TemporalLeylineEnergyDensity.Num(); ++Index)
		{
			TemporalLeylineEnergyDensity[Index] = FMath::Sin(Index * TimeStep);
		}
	}

	if (EnergyGradient.Num() == 0)
	{
		EnergyGradient.SetNumZeroed(TemporalLeylineEnergyDensity.Num());
		for (int32 Index = 0; Index < EnergyGradient.Num(); ++Index)
		{
			EnergyGradient[Index] = FMath::Cos(Index * TimeStep);
		}
	}
}

double ULeylineEnergyInfusion9Component::ComputeLeylineEnergyFlow() const
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

void ULeylineEnergyInfusion9Component::LogResults(double Power) const
{
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Infusion (P_nuclear): %.4f"), Power);
	UE_LOG(LogTemp, Log, TEXT("[TensorFlow Validation] Leyline Energy Infusion (P_nuclear): %.4f"), Power);
}

void ULeylineEnergyInfusion9Component::RecalculateLeylineEnergy()
{
	if (TemporalLeylineEnergyDensity.Num() != EnergyGradient.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Leyline arrays must have the same length."));
		NuclearPower = 0.0;
		return;
	}

	NuclearPower = ComputeLeylineEnergyFlow();
	LogResults(NuclearPower);
}
