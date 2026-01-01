#include "EnergyConversionEfficiencyComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyConversionEfficiencyComponent::UEnergyConversionEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	EnergyOutput = 400.0;
	EnergyInput = 500.0;
	Efficiency = 0.0;
}

void UEnergyConversionEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeParameters();
	Efficiency = CalculateEfficiency(EnergyOutput, EnergyInput);
	UE_LOG(LogTemp, Log, TEXT("Efficiency (η): %.2f%%"), Efficiency * 100.0);

	TArray<double> Prediction = PredictEfficiency(EnergyOutput, EnergyInput);
	if (Prediction.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Efficiency (η): %.2f%%"), Prediction[0] * 100.0);
	}

	TensorFlowModeling(EnergyOutput, EnergyInput);
}

void UEnergyConversionEfficiencyComponent::InitializeParameters()
{
	Efficiency = 0.0;
}

double UEnergyConversionEfficiencyComponent::CalculateEfficiency(double Output, double Input) const
{
	if (FMath::IsNearlyZero(Input))
	{
		UE_LOG(LogTemp, Error, TEXT("Energy input cannot be zero."));
		return 0.0;
	}
	return Output / Input;
}

TArray<double> UEnergyConversionEfficiencyComponent::PredictEfficiency(double Output, double Input) const
{
	double Sum = Output * 0.7 - Input * 0.3;
	double Sigmoid = 1.0 / (1.0 + FMath::Exp(-Sum / 100.0));
	return { Sigmoid };
}

void UEnergyConversionEfficiencyComponent::TensorFlowModeling(double Output, double Input) const
{
	double Result = CalculateEfficiency(Output, Input);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computed Efficiency (η): %.2f%%"), Result * 100.0);
}
