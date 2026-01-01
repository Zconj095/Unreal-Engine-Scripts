#include "NeuralPlasticityGeneticProcessingComponent.h"
#include "Logging/LogMacros.h"

UNeuralPlasticityGeneticProcessingComponent::UNeuralPlasticityGeneticProcessingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralPlasticityGeneticProcessingComponent::BeginPlay()
{
	Super::BeginPlay();

	NeuralPlasticity = CalculateNeuralPlasticity(PlasticityCoefficient, EnvironmentalStimuli, GeneticStimuli);
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity in Genetic Processing (P_g): %0.4f"), NeuralPlasticity);
}

float UNeuralPlasticityGeneticProcessingComponent::CalculateNeuralPlasticity(float Kappa, float E, float G) const
{
	return Kappa * (E + G);
}
