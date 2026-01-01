#include "NeuralSignalTransmissionComponent.h"
#include "Logging/LogMacros.h"

UNeuralSignalTransmissionComponent::UNeuralSignalTransmissionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralSignalTransmissionComponent::BeginPlay()
{
	Super::BeginPlay();

	ComputeNeuralSignalVelocity();
}

void UNeuralSignalTransmissionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ComputeNeuralSignalVelocity();
}

void UNeuralSignalTransmissionComponent::ComputeNeuralSignalVelocity()
{
	if (QuantumWaveFrequency <= 0.0f || QuantumEntanglementProbability <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Quantum wave frequency and quantum entanglement probability must be greater than zero."));
		return;
	}

	const float Denominator = MembraneResistance + (1.0f / (QuantumWaveFrequency * QuantumEntanglementProbability));
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Error, TEXT("Computed denominator is zero; cannot divide by zero."));
		return;
	}

	NeuralSignalVelocity = DeltaQ / Denominator;
	UE_LOG(LogTemp, Log, TEXT("Neural Signal Velocity (v_n): %0.4f"), NeuralSignalVelocity);
}

float UNeuralSignalTransmissionComponent::GetNeuralSignalVelocity() const
{
	return NeuralSignalVelocity;
}
