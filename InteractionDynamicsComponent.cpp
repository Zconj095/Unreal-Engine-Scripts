#include "InteractionDynamicsComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

UInteractionDynamicsComponent::UInteractionDynamicsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionDynamicsComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateInteractionEnergy();
}

void UInteractionDynamicsComponent::InitializeParameters()
{
	InteractionEnergy = 0.0;
}

double UInteractionDynamicsComponent::CalculateInteractionEnergy() const
{
	double TotalEnergy = 0.0;
	const double TimeStepDouble = FMath::Max(TimeStep, KINDA_SMALL_NUMBER);

	for (double Time = 0.0; Time <= EndTime; Time += TimeStepDouble)
	{
		TotalEnergy += EngagementPower * FMath::Cos(Frequency * Time + PhaseOffset) * TimeStepDouble;
	}

	return TotalEnergy;
}

double UInteractionDynamicsComponent::PredictInteractionEnergyWithMockNetwork(double Value) const
{
	return Value * 0.92;
}

double UInteractionDynamicsComponent::ComputeInteractionEnergyWithTensorFlowStub() const
{
	return CalculateInteractionEnergy();
}

void UInteractionDynamicsComponent::LogResults(double Energy, double NetworkPrediction, double TensorFlowValue) const
{
	UE_LOG(LogTemp, Log, TEXT("Interaction Energy at time t (I_t): %.4f"), Energy);
	UE_LOG(LogTemp, Log, TEXT("Predicted Interaction Energy (I_t): %.4f"), NetworkPrediction);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (I_t): %.4f"), TensorFlowValue);
}

void UInteractionDynamicsComponent::RecalculateInteractionEnergy()
{
	InitializeParameters();

	const double ComputedEnergy = CalculateInteractionEnergy();
	const double NeuralPrediction = PredictInteractionEnergyWithMockNetwork(ComputedEnergy);
	const double TensorFlowValidation = ComputeInteractionEnergyWithTensorFlowStub();

	InteractionEnergy = ComputedEnergy;
	LogResults(ComputedEnergy, NeuralPrediction, TensorFlowValidation);
}
