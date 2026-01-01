#include "ResonanceResponseVisualizerComponent.h"

#include "DrawDebugHelpers.h"
#include "Logging/LogMacros.h"
#include <complex>

namespace
{
	static const double LocalPi = 3.14159265358979323846;
	static const double LocalTwoPi = 2.0 * LocalPi;
}

UResonanceResponseVisualizerComponent::UResonanceResponseVisualizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResonanceResponseVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (NumFrequencies <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NumFrequencies must be greater than zero."));
		return;
	}

	for (int32 Index = 0; Index < NumFrequencies; ++Index)
	{
		const float Frequency = FrequencyStart + Index;
		VisualizeFrequency(Frequency, Index);
	}
}

void UResonanceResponseVisualizerComponent::VisualizeFrequency(float Frequency, int32 Index)
{
	const FComplex Resonance = ComputeResonance(Frequency);
	const float Height = FMath::Sqrt(FMath::Square(Resonance.Real) + FMath::Square(Resonance.Imag));
	const FVector Position = GetOwner()->GetActorLocation() + FVector(Index * FrequencySpacing, 0.0f, Height * HeightScale);

	DrawDebugSphere(GetWorld(), Position, 20.0f, 12, PointColor, true, 0.0f, 0, 2.0f);
	if (bLogValues)
	{
		UE_LOG(LogTemp, Log, TEXT("Frequency %.2fHz -> |Resonance|=%.4f"), Frequency, Height);
	}
}

UResonanceResponseVisualizerComponent::FComplex UResonanceResponseVisualizerComponent::ComputeResonance(float Frequency) const
{
	const double Step = 1.0;
	std::complex<double> Accum(0.0, 0.0);
	for (double X = 0.0; X <= NumFrequencies; ++X)
	{
		const double Density = EvaluateDensity(X);
		const double Angle = -LocalTwoPi * Frequency * X;
		const double CosAngle = static_cast<double>(FMath::Cos(static_cast<float>(Angle)));
		const double SinAngle = static_cast<double>(FMath::Sin(static_cast<float>(Angle)));
		std::complex<double> Exponential(CosAngle, SinAngle);
		Accum += std::complex<double>(Density, 0.0) * Exponential * Step;
	}

	return { static_cast<float>(Accum.real()), static_cast<float>(Accum.imag()) };
}

float UResonanceResponseVisualizerComponent::EvaluateDensity(float Frequency) const
{
	return static_cast<float>(FMath::Exp(-Frequency) * FMath::Sin(2.0f * LocalPi * Frequency));
}
