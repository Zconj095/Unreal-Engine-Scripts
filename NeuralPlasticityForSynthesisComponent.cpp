#include "NeuralPlasticityForSynthesisComponent.h"
#include "Logging/LogMacros.h"

UNeuralPlasticityForSynthesisComponent::UNeuralPlasticityForSynthesisComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPlasticityForSynthesisComponent::BeginPlay()
{
	Super::BeginPlay();

	RecalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (Pₙ): %0.4f"), NeuralPlasticityValue);
}

void UNeuralPlasticityForSynthesisComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RecalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (Pₙ): %0.4f"), NeuralPlasticityValue);
}

float UNeuralPlasticityForSynthesisComponent::CalculatePlasticity(float Kappa, float Nc, float Sb) const
{
	return Kappa * (Nc + Sb);
}

void UNeuralPlasticityForSynthesisComponent::SetPlasticityCoefficient(float Value)
{
	PlasticityCoefficient = Value;
	RecalculatePlasticity();
}

void UNeuralPlasticityForSynthesisComponent::SetCognitiveStimuli(float Value)
{
	CognitiveStimuli = Value;
	RecalculatePlasticity();
}

void UNeuralPlasticityForSynthesisComponent::SetBiochemicalStimuli(float Value)
{
	BiochemicalStimuli = Value;
	RecalculatePlasticity();
}

void UNeuralPlasticityForSynthesisComponent::RecalculatePlasticity()
{
	NeuralPlasticityValue = CalculatePlasticity(PlasticityCoefficient, CognitiveStimuli, BiochemicalStimuli);
}
