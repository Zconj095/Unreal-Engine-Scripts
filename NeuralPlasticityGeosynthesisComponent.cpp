#include "NeuralPlasticityGeosynthesisComponent.h"
#include "Logging/LogMacros.h"

UNeuralPlasticityGeosynthesisComponent::UNeuralPlasticityGeosynthesisComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPlasticityGeosynthesisComponent::BeginPlay()
{
	Super::BeginPlay();

	RecalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (Pg): %0.2f"), NeuralPlasticity);
}

void UNeuralPlasticityGeosynthesisComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RecalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (Pg): %0.2f"), NeuralPlasticity);
}

float UNeuralPlasticityGeosynthesisComponent::CalculateNeuralPlasticity(float Sg, float Gm) const
{
	return PlasticityCoefficient * (Sg + Gm);
}

void UNeuralPlasticityGeosynthesisComponent::RecalculatePlasticity()
{
	NeuralPlasticity = CalculateNeuralPlasticity(GeologicalStimuli, GeomorphicStimuli);
}
