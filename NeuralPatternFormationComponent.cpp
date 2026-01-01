#include "NeuralPatternFormationComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralPatternFormationComponent::UNeuralPatternFormationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralPatternFormationComponent::BeginPlay()
{
	Super::BeginPlay();

	LogPatternFormationPotential();
}

float UNeuralPatternFormationComponent::CalculatePatternFormationPotential()
{
	const int32 EffectiveSamples = FMath::Clamp(NumSamples, 10, 10000);
	const float Dx = (EndTime - StartTime) / EffectiveSamples;
	double Integral = 0.0;

	for (int32 Index = 0; Index < EffectiveSamples; ++Index)
	{
		const float CurrentTime = StartTime + Index * Dx;
		const float X = Index * Dx;

		const float Wavefunction = NeuralWavefunction(X, CurrentTime);
		const float Hamiltonian = HamiltonianOperator(X, CurrentTime);

		Integral += Wavefunction * Hamiltonian * Wavefunction * Dx;
	}

	return static_cast<float>(Integral);
}

void UNeuralPatternFormationComponent::LogPatternFormationPotential()
{
	const float Potential = CalculatePatternFormationPotential();
	UE_LOG(LogTemp, Log, TEXT("Neural Pattern Formation Potential (Pₓ): %.4f"), Potential);
}

float UNeuralPatternFormationComponent::NeuralWavefunction(float X, float T) const
{
	return FMath::Exp(-X * X - T * T);
}

float UNeuralPatternFormationComponent::HamiltonianOperator(float X, float T) const
{
	return X * X + T * T;
}
