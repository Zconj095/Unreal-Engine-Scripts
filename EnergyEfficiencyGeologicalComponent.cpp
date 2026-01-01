#include "EnergyEfficiencyGeologicalComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyEfficiencyGeologicalComponent::UEnergyEfficiencyGeologicalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BoltzmannConstant = 1.38e-23f;
	NeuralTemperature = 310.0f;
	ProcessingEfficiency = 1.0f;
	SynapticEnergy = 0.0f;
}

void UEnergyEfficiencyGeologicalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SynapticEnergy = CalculateEEGC(BoltzmannConstant, NeuralTemperature, ProcessingEfficiency);
	UE_LOG(LogTemp, Log, TEXT("Synaptic Energy (Es): %e J"), SynapticEnergy);
}

float UEnergyEfficiencyGeologicalComponent::CalculateEEGC(float kB, float Temperature, float Efficiency) const
{
	constexpr float Ln2 = 0.69314718f;
	return (kB * Temperature / Ln2) * Efficiency;
}
