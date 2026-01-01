#include "EnergyFlowAnalysisComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyFlowAnalysisComponent::UEnergyFlowAnalysisComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HeatAdded = 100.0f;
	WorkDone = 50.0f;
	MagicalCoefficient = 1.5f;
	MagicalFlux = 20.0f;
	DeltaEnergy = 0.0f;
}

void UEnergyFlowAnalysisComponent::BeginPlay()
{
	Super::BeginPlay();

	DeltaEnergy = CalculateEnergyChange(HeatAdded, WorkDone, MagicalCoefficient, MagicalFlux);
	UE_LOG(LogTemp, Log, TEXT("Delta E (Change in Energy): %f"), DeltaEnergy);

	TArray<float> Prediction = PredictEnergyFlow(HeatAdded, WorkDone, MagicalCoefficient, MagicalFlux);
	if (Prediction.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Delta E: %f"), Prediction[0]);
	}

	const float TensorFlowResult = ComputeWithTensorFlow(HeatAdded, WorkDone, MagicalCoefficient, MagicalFlux);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result: %f"), TensorFlowResult);
}

float UEnergyFlowAnalysisComponent::CalculateEnergyChange(float Heat, float Work, float Mu, float Flux) const
{
	return Heat - Work + Mu * Flux;
}

TArray<float> UEnergyFlowAnalysisComponent::PredictEnergyFlow(float Heat, float Work, float Mu, float Flux) const
{
	const float Sum = Heat * 0.6f - Work * 0.4f + Mu * Flux * 0.1f;
	const float Sigmoid = 1.0f / (1.0f + FMath::Exp(-Sum / 100.0f));
	return { Sigmoid };
}

float UEnergyFlowAnalysisComponent::ComputeWithTensorFlow(float Heat, float Work, float Mu, float Flux) const
{
	return CalculateEnergyChange(Heat, Work, Mu, Flux);
}
