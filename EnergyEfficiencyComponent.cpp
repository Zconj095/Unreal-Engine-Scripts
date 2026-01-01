#include "EnergyEfficiencyComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyEfficiencyComponent::UEnergyEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	OutputPower = 100.0f;
	InputPower = 150.0f;
	MagicalEnergyField = 50.0f;
	MagicalContribution = 0.2f;
	Efficiency = 0.0f;
}

void UEnergyEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	Efficiency = CalculateEnergyEfficiency(OutputPower, InputPower, MagicalEnergyField, MagicalContribution);
	UE_LOG(LogTemp, Log, TEXT("Energy Efficiency (η_e): %.4f"), Efficiency);
	TensorFlowValidation(OutputPower, InputPower, MagicalEnergyField, MagicalContribution);
}

float UEnergyEfficiencyComponent::CalculateEnergyEfficiency(float POutput, float PInput, float Field, float Contribution) const
{
	const float Denominator = PInput + Contribution * Field;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Error, TEXT("Denominator is zero, energy efficiency cannot be calculated!"));
		return 0.0f;
	}

	return POutput / Denominator;
}

void UEnergyEfficiencyComponent::TensorFlowValidation(float POutput, float PInput, float Field, float Contribution) const
{
	const float Result = CalculateEnergyEfficiency(POutput, PInput, Field, Contribution);
	UE_LOG(LogTemp, Log, TEXT("[TensorFlow Validation] Energy Efficiency (η_e): %.4f"), Result);
}
