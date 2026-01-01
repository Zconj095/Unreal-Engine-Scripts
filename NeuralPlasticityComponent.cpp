#include "NeuralPlasticityComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralPlasticityComponent::UNeuralPlasticityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralPlasticityComponent::BeginPlay()
{
	Super::BeginPlay();

	const float PlasticityValue = CalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity for Psychological Adaptation (P_p): %.4f"), PlasticityValue);
}

float UNeuralPlasticityComponent::CalculatePlasticity() const
{
	return PlasticityCoefficient * (CognitiveStimuli + EmotionalStimuli);
}
