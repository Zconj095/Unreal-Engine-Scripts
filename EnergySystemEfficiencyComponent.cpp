#include "EnergySystemEfficiencyComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergySystemEfficiencyComponent::UEnergySystemEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	WorkOutput = 100.0;
	HeatInput = 200.0;
	MagicalEnergyFlux = 50.0;
	Efficiency = 0.0;
}

void UEnergySystemEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	Efficiency = CalculateEfficiency(WorkOutput, HeatInput, MagicalEnergyFlux);
	UE_LOG(LogTemp, Log, TEXT("System Efficiency (η): %.2f%%"), Efficiency * 100.0);
	PredictEfficiencyWithNeuralNetwork();
	TensorFlowModeling();
}

double UEnergySystemEfficiencyComponent::CalculateEfficiency(double Work, double Heat, double Magical) const
{
	double Denominator = Heat + Magical;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Denominator (Q_in + μΦ) is zero."));
		return 0.0;
	}
	return Work / Denominator;
}

void UEnergySystemEfficiencyComponent::PredictEfficiencyWithNeuralNetwork() const
{
	// Placeholder logging for neural net result
	double Prediction = CalculateEfficiency(WorkOutput, HeatInput, MagicalEnergyFlux) * 0.97;
	UE_LOG(LogTemp, Log, TEXT("Predicted Efficiency (η): %.2f%%"), Prediction * 100.0);
}

void UEnergySystemEfficiencyComponent::TensorFlowModeling() const
{
	double Result = CalculateEfficiency(WorkOutput, HeatInput, MagicalEnergyFlux);
	UE_LOG(LogTemp, Log, TEXT("TensorFlowSharp Calculated Efficiency (η): %.2f%%"), Result * 100.0);
}
