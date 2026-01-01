#include "NeuralQuantumEntanglementComponent.h"
#include "Logging/LogMacros.h"

UNeuralQuantumEntanglementComponent::UNeuralQuantumEntanglementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralQuantumEntanglementComponent::BeginPlay()
{
	Super::BeginPlay();

	NeuralQuantumEntanglementValue = CalculateGamma();
	UE_LOG(LogTemp, Log, TEXT("Neural Quantum Entanglement (E_q): %0.4f"), NeuralQuantumEntanglementValue);
}

float UNeuralQuantumEntanglementComponent::CalculateGamma() const
{
	const int32 Steps = FMath::Max(IntegrationSteps, 1);
	const float Range = EndTime - StartTime;
	const float Dx = Range / Steps;
	float Sum = 0.0f;

	for (int32 Step = 0; Step < Steps; ++Step)
	{
		const float X = StartTime + Step * Dx;
		const float WaveProduct = Psi1(X, EndTime) * Psi2(X, EndTime) * ComplexConjugate(Psi2(X, EndTime));
		Sum += FMath::Pow(WaveProduct, 2) * Dx;
	}

	return Sum;
}

float UNeuralQuantumEntanglementComponent::Psi1(float X, float T) const
{
	return FMath::Sin(X + T);
}

float UNeuralQuantumEntanglementComponent::Psi2(float X, float T) const
{
	return FMath::Cos(X - T);
}

float UNeuralQuantumEntanglementComponent::ComplexConjugate(float Value) const
{
	return Value;
}
