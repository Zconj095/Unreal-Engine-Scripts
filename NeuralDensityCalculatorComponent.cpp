#include "NeuralDensityCalculatorComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"

UNeuralDensityCalculatorComponent::UNeuralDensityCalculatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralDensityCalculatorComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetDensity();
}

void UNeuralDensityCalculatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateNeuralDensity();
}

void UNeuralDensityCalculatorComponent::UpdateNeuralDensity()
{
	if (BrainVolume <= KINDA_SMALL_NUMBER)
	{
		LogVolumeError();
		NeuralDensity = 0.0f;
		return;
	}

	NeuralDensity = CalculateNeuralDensity(NeuronsPerUnitVolume, BrainVolume, QuantumAdaptationFactor);
	bHasLoggedVolumeError = false;
}

float UNeuralDensityCalculatorComponent::CalculateNeuralDensity(float NeuronsPerUnitVolumeParam, float BrainVolumeParam, float QuantumFactorParam) const
{
	if (BrainVolumeParam <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	return (NeuronsPerUnitVolumeParam / BrainVolumeParam) * QuantumFactorParam;
}

void UNeuralDensityCalculatorComponent::ResetDensity()
{
	NeuralDensity = 0.0f;
	bHasLoggedVolumeError = false;
}

void UNeuralDensityCalculatorComponent::LogVolumeError()
{
	if (bHasLoggedVolumeError)
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Brain volume must be greater than zero to calculate neural density."));
	bHasLoggedVolumeError = true;
}
