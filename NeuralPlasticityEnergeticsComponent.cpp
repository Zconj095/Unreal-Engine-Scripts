#include "NeuralPlasticityEnergeticsComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralPlasticityEnergeticsComponent::UNeuralPlasticityEnergeticsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralPlasticityEnergeticsComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity Energetics (P_e): %.4f"), EnergeticsPlasticity);
}

void UNeuralPlasticityEnergeticsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePlasticity();
}

void UNeuralPlasticityEnergeticsComponent::UpdatePlasticity()
{
	EnergeticsPlasticity = PlasticityCoefficient * (EnvironmentalEnergy + InternalEnergy);
}
