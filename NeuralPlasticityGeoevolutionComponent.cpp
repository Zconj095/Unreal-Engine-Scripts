#include "NeuralPlasticityGeoevolutionComponent.h"

UNeuralPlasticityGeoevolutionComponent::UNeuralPlasticityGeoevolutionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPlasticityGeoevolutionComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity for Geoevolution (P_g): %.4f"), CalculatedPlasticity);
}

void UNeuralPlasticityGeoevolutionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdatePlasticity();
}

void UNeuralPlasticityGeoevolutionComponent::UpdatePlasticity()
{
	CalculatedPlasticity = PlasticityCoefficient * (EnvironmentalStimuli + GeneticEncoding);
}

float UNeuralPlasticityGeoevolutionComponent::CalculatePlasticity() const
{
	return CalculatedPlasticity;
}
