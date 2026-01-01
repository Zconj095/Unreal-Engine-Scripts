#include "NeuralPlasticityBiosynthesisComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralPlasticityBiosynthesisComponent::UNeuralPlasticityBiosynthesisComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralPlasticityBiosynthesisComponent::BeginPlay()
{
	Super::BeginPlay();

	const float PlasticityValue = CalculatePlasticity();
	UE_LOG(LogTemp, Log, TEXT("Neural Plasticity for Biosynthesis (P_b): %.4f"), PlasticityValue);
}

float UNeuralPlasticityBiosynthesisComponent::CalculatePlasticity() const
{
	return PlasticityCoefficient * (MolecularStimuli + BiochemicalStimuli);
}
