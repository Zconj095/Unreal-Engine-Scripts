#include "QuantumLearningRateComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UQuantumLearningRateComponent::UQuantumLearningRateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumLearningRateComponent::BeginPlay()
{
	Super::BeginPlay();

	if (NeuralPlasticity < 0.0f || NeuralPlasticity > 1.0f || QuantumCoherence < 0.0f || QuantumCoherence > 1.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Neural plasticity (P) and quantum coherence (Q) must be between 0 and 1!"));
		LearningRate = 0.0f;
	}
	else
	{
		LearningRate = EfficiencyConstant * NeuralPlasticity * QuantumCoherence;
	}

	UE_LOG(LogTemp, Log, TEXT("Quantum Learning Rate (L): %0.4f"), LearningRate);
}
