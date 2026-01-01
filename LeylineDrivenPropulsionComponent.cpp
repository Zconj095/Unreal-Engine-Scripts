#include "LeylineDrivenPropulsionComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineDrivenPropulsionComponent::ULeylineDrivenPropulsionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineDrivenPropulsionComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculatePower();
}

double ULeylineDrivenPropulsionComponent::EnergyDensity(double Time) const
{
	return FMath::Sin(Time);
}

double ULeylineDrivenPropulsionComponent::LeylineGradient(double Time) const
{
	return FMath::Cos(Time);
}

double ULeylineDrivenPropulsionComponent::CalculateLeylineEnergyFlow() const
{
	const int32 Points = FMath::Max(IntegrationPoints, 1);
	const double TimeStep = (EndTime - StartTime) / Points;
	double Sum = 0.0;

	for (int32 Index = 0; Index <= Points; ++Index)
	{
		const double Time = StartTime + Index * TimeStep;
		const double Integrand = EnergyDensity(Time) * LeylineGradient(Time);
		const double Weight = (Index == 0 || Index == Points) ? 0.5 : 1.0;
		Sum += Integrand * Weight;
	}

	return Sum * TimeStep;
}

void ULeylineDrivenPropulsionComponent::RecalculatePower()
{
	PowerDelivered = CalculateLeylineEnergyFlow();
	UE_LOG(LogTemp, Log, TEXT("Power Delivered (P_air): %.4f"), PowerDelivered);

	const double NeuralPrediction = PredictEnergyFlowWithNeuralNetwork();
	UE_LOG(LogTemp, Log, TEXT("Predicted Power Delivered (P_air): %.4f"), NeuralPrediction);

	const double TensorFlowResult = TensorFlowModelingPlaceholder();
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Calculated Power Delivered (P_air): %.4f"), TensorFlowResult);
}

double ULeylineDrivenPropulsionComponent::PredictEnergyFlowWithNeuralNetwork() const
{
	return PowerDelivered * 1.1;
}

double ULeylineDrivenPropulsionComponent::TensorFlowModelingPlaceholder() const
{
	const int32 Steps = FMath::Max(IntegrationPoints / 10, 1);
	const double Step = (EndTime - StartTime) / Steps;
	double Accumulated = 0.0;

	for (int32 Index = 0; Index <= Steps; ++Index)
	{
		const double Time = StartTime + Index * Step;
		Accumulated += EnergyDensity(Time) * LeylineGradient(Time);
	}

	return Accumulated * Step;
}
