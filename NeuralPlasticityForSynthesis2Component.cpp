#include "NeuralPlasticityForSynthesis2Component.h"
#include "Logging/LogMacros.h"

UNeuralPlasticityForSynthesis2Component::UNeuralPlasticityForSynthesis2Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPlasticityForSynthesis2Component::BeginPlay()
{
	Super::BeginPlay();

	RecalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (Pn): %0.4f"), NeuralPlasticityValue);
}

void UNeuralPlasticityForSynthesis2Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RecalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (Pn): %0.4f"), NeuralPlasticityValue);
}

float UNeuralPlasticityForSynthesis2Component::CalculatePlasticity(float Kappa, float Nc, float Sb) const
{
	return Kappa * (Nc + Sb);
}

void UNeuralPlasticityForSynthesis2Component::SetPlasticityCoefficient(float Value)
{
	PlasticityCoefficient = Value;
	RecalculatePlasticity();
}

void UNeuralPlasticityForSynthesis2Component::SetCognitiveStimuli(float Value)
{
	CognitiveStimuli = Value;
	RecalculatePlasticity();
}

void UNeuralPlasticityForSynthesis2Component::SetBiochemicalStimuli(float Value)
{
	BiochemicalStimuli = Value;
	RecalculatePlasticity();
}

void UNeuralPlasticityForSynthesis2Component::RecalculatePlasticity()
{
	NeuralPlasticityValue = CalculatePlasticity(PlasticityCoefficient, CognitiveStimuli, BiochemicalStimuli);
}
