#include "MaterialResonanceAnalysisActor.h"
#include "Math/UnrealMathUtility.h"

AMaterialResonanceAnalysisActor::AMaterialResonanceAnalysisActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMaterialResonanceAnalysisActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeResonance();
}

FMaterialComplex DensityFunction(double X)
{
	FMaterialComplex Value;
	Value.Real = FMath::Exp(-X) * FMath::Cos(2 * PI * X);
	Value.Imag = 0.0;
	return Value;
}

FMaterialComplex MultiplyComplex(const FMaterialComplex& A, const FMaterialComplex& B)
{
	return {
		A.Real * B.Real - A.Imag * B.Imag,
		A.Real * B.Imag + A.Imag * B.Real
	};
}

FMaterialComplex MaterialResonanceIntegrand(double Frequency, double X)
{
	FMaterialComplex Rho = DensityFunction(X);
	const double Angle = -2.0 * PI * Frequency * X;
	FMaterialComplex Exponential;
	Exponential.Real = FMath::Cos(Angle);
	Exponential.Imag = FMath::Sin(Angle);
	return MultiplyComplex(Rho, Exponential);
}

FMaterialComplex AMaterialResonanceAnalysisActor::ComputeIntegration(double InFrequency) const
{
	FMaterialComplex Result{0.0, 0.0};
	const double Step = (UpperBound - 0.0) / Steps;

	for (int32 Index = 0; Index < Steps; ++Index)
	{
		const double X = Index * Step;
		FMaterialComplex Integrand = MaterialResonanceIntegrand(InFrequency, X);
		Result.Real += Integrand.Real * Step;
		Result.Imag += Integrand.Imag * Step;
	}

	return Result;
}

void AMaterialResonanceAnalysisActor::ComputeResonance() const
{
	FMaterialComplex Resonance = ComputeIntegration(Frequency);
	UE_LOG(LogTemp, Log, TEXT("Resonance Response at f=%.2f: %.4f + %.4fj"), Frequency, Resonance.Real, Resonance.Imag);
}
