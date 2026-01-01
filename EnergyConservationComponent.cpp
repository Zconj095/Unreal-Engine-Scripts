#include "EnergyConservationComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyConservationComponent::UEnergyConservationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HeatInput = 100.0;
	WorkDone = 50.0;
	MagicEnergy = 10.0;
	ChangeInInternalEnergy = 0.0;
}

void UEnergyConservationComponent::BeginPlay()
{
	Super::BeginPlay();

	ChangeInInternalEnergy = CalculateEnergyConservation(HeatInput, WorkDone, MagicEnergy);
	UE_LOG(LogTemp, Log, TEXT("Change in Internal Energy (ΔU): %f"), ChangeInInternalEnergy);
	UE_LOG(LogTemp, Log, TEXT("Neural prediction ΔU: %f"), PredictEnergyWithNeuralNetwork());
	TensorFlowModeling();
}

double UEnergyConservationComponent::CalculateEnergyConservation(double Q, double W, double MagicalEnergy) const
{
	return Q - W + MagicalEnergy;
}

double UEnergyConservationComponent::PredictEnergyWithNeuralNetwork() const
{
	// Simplified placeholder calculation mimicking neural network behavior
	double Sum = HeatInput * 0.5 + WorkDone * -0.3 + MagicEnergy * 0.2;
	return 1.0 / (1.0 + FMath::Exp(-Sum / 100.0));
}

void UEnergyConservationComponent::TensorFlowModeling() const
{
	// Placeholder log to keep parity with original example
	double Result = CalculateEnergyConservation(HeatInput, WorkDone, MagicEnergy);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Calculated Change in Internal Energy (ΔU): %f"), Result);
}
