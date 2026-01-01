#include "NeuralParticleSymmetryComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralParticleSymmetryComponent::UNeuralParticleSymmetryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralParticleSymmetryComponent::BeginPlay()
{
	Super::BeginPlay();

	LogSymmetryExpectationValue();
}

float UNeuralParticleSymmetryComponent::CalculateSymmetryOperatorExpectationValue() const
{
	const int32 EffectiveSteps = FMath::Max(IntegrationSteps, 2);
	const float Dx = (EndX - StartX) / EffectiveSteps;

	double Sum = 0.0;

	for (int32 Index = 0; Index < EffectiveSteps; ++Index)
	{
		const float X = StartX + Index * Dx;
		const FElvenGoddessComplex Psi = WaveFunction(X, TimeParameter);
		const FElvenGoddessComplex PsiConjugate = Psi.Conjugate();
		const FElvenGoddessComplex Symmetry = SymmetryOperator(X, TimeParameter, Psi);

		const double RealPart = PsiConjugate.Real * Symmetry.Real - PsiConjugate.Imaginary * Symmetry.Imaginary;

		Sum += RealPart * Dx;
	}

	return static_cast<float>(Sum);
}

void UNeuralParticleSymmetryComponent::LogSymmetryExpectationValue() const
{
	const float Value = CalculateSymmetryOperatorExpectationValue();
	UE_LOG(LogTemp, Log, TEXT("Symmetry Operator Expectation Value (S_p): %.4f"), Value);
}

FElvenGoddessComplex UNeuralParticleSymmetryComponent::WaveFunction(float X, float TimeParam) const
{
	const float K = 1.0f;
	const float Omega = 1.0f;
	const float Phase = K * X - Omega * TimeParam;

	return FElvenGoddessComplex::FromPolarCoordinates(1.0f, Phase);
}

FElvenGoddessComplex UNeuralParticleSymmetryComponent::SymmetryOperator(float X, float TimeParam, const FElvenGoddessComplex& Psi) const
{
	return FElvenGoddessComplex(-Psi.Real, -Psi.Imaginary);
}
