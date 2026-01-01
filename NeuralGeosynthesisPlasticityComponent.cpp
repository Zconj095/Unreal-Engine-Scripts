#include "NeuralGeosynthesisPlasticityComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralGeosynthesisPlasticityComponent::UNeuralGeosynthesisPlasticityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralGeosynthesisPlasticityComponent::BeginPlay()
{
	Super::BeginPlay();
	const float Plasticity = CalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (P_g): %.4f"), Plasticity);
}

float UNeuralGeosynthesisPlasticityComponent::CalculatePlasticity() const
{
	return PlasticityCoefficient * (EnvironmentalStimuli + GeologicalMemory);
}
