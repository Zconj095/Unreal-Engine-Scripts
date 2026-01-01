#include "GeneticResonanceAnalysisActor.h"

#include "Math/UnrealMathUtility.h"
#include <complex>

constexpr double GRA_PI = 3.14159265358979323846;
constexpr double GRA_TWO_PI = 2.0 * GRA_PI;

AGeneticResonanceAnalysisActor::AGeneticResonanceAnalysisActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticResonanceAnalysisActor::BeginPlay()
{
	Super::BeginPlay();

	if (Points <= 0 || UpperBound <= LowerBound)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid integration bounds or point count."));
		return;
	}

	FComplex Result = ComputeResonance(Frequency);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Resonance Response at f=%.2fHz: %.6f + %.6fj"), Frequency, Result.Real, Result.Imag);
	}
}

AGeneticResonanceAnalysisActor::FComplex AGeneticResonanceAnalysisActor::ComputeResonance(double InFrequency) const
{
	double Step = (UpperBound - LowerBound) / Points;
	std::complex<double> Accum(0.0, 0.0);

	for (int32 Index = 0; Index < Points; ++Index)
	{
		double X = LowerBound + Index * Step;
		double Density = EvaluateDensity(X);
		const double Angle = -GRA_TWO_PI * InFrequency * X;
		const double CosAngle = static_cast<double>(FMath::Cos(static_cast<float>(Angle)));
		const double SinAngle = static_cast<double>(FMath::Sin(static_cast<float>(Angle)));
		std::complex<double> Exponential(CosAngle, SinAngle);
		Accum += std::complex<double>(Density, 0.0) * Exponential * Step;
	}

	return { Accum.real(), Accum.imag() };
}

double AGeneticResonanceAnalysisActor::EvaluateDensity(double X) const
{
	const float Exponent = static_cast<float>(-X);
	const float Angle = static_cast<float>(2.0 * GRA_PI * X);
	return static_cast<double>(FMath::Exp(Exponent) * FMath::Sin(Angle));
}
