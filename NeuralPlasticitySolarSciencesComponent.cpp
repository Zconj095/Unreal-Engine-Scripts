#include "NeuralPlasticitySolarSciencesComponent.h"
#include "Logging/LogMacros.h"

UNeuralPlasticitySolarSciencesComponent::UNeuralPlasticitySolarSciencesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPlasticitySolarSciencesComponent::BeginPlay()
{
	Super::BeginPlay();

	RecalculatePlasticity();
}

void UNeuralPlasticitySolarSciencesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RecalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity (Ps): %0.2f"), NeuralPlasticity);
}

float UNeuralPlasticitySolarSciencesComponent::CalculateNPSS(float Kappa, float Eg, float Rs) const
{
	return Kappa * (Eg + Rs);
}

void UNeuralPlasticitySolarSciencesComponent::RecalculatePlasticity()
{
	NeuralPlasticity = CalculateNPSS(PlasticityCoefficient, EnvironmentalStimuli, RadiativeStimuli);
}
