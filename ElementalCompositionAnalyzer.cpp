#include "ElementalCompositionAnalyzer.h"
#include "Math/UnrealMathUtility.h"

TArray<double> UElementalCompositionAnalyzer::GenerateElementalDensity(int32 SampleSize, double Dx)
{
	TArray<double> Density;
	const int32 ValidSamples = FMath::Max(1, SampleSize);
	Density.SetNum(ValidSamples);

	for (int32 Index = 0; Index < ValidSamples; ++Index)
	{
		const double Position = Index * Dx;
		const double Noise = FMath::FRandRange(0.0, 0.1);
		Density[Index] = FMath::Sin(Position) + FMath::Cos(2.0 * Position) + Noise;
	}

	return Density;
}

TArray<FFrequencyIntensity> UElementalCompositionAnalyzer::ComputeIntensity(const TArray<double>& Density) const
{
	TArray<FFrequencyIntensity> Intensity;
	const int32 Count = Density.Num();
	if (Count == 0)
	{
		return Intensity;
	}

	Intensity.SetNum(Count);
	const double TwoPi = 2.0 * PI;

	for (int32 Frequency = 0; Frequency < Count; ++Frequency)
	{
		double Real = 0.0;
		double Imag = 0.0;

		for (int32 Sample = 0; Sample < Count; ++Sample)
		{
			const double Angle = TwoPi * Frequency * Sample / Count;
			const double Value = Density[Sample];
			Real += Value * FMath::Cos(Angle);
			Imag -= Value * FMath::Sin(Angle);
		}

		Intensity[Frequency].Real = Real;
		Intensity[Frequency].Imag = Imag;
	}

	return Intensity;
}

void UElementalCompositionAnalyzer::Test()
{
	const int32 SampleSize = 1024;
	const double Dx = 0.01;

	UElementalCompositionAnalyzer* Analyzer = NewObject<UElementalCompositionAnalyzer>();
	const TArray<double> Density = Analyzer->GenerateElementalDensity(SampleSize, Dx);
	const TArray<FFrequencyIntensity> Spectrum = Analyzer->ComputeIntensity(Density);

	UE_LOG(LogTemp, Log, TEXT("Intensity Spectrum:"));
	const int32 DisplayCount = FMath::Min(10, Spectrum.Num());
	for (int32 Index = 0; Index < DisplayCount; ++Index)
	{
		const double Magnitude = Spectrum[Index].Magnitude();
		UE_LOG(LogTemp, Log, TEXT("Frequency %d: Magnitude = %.4f"), Index, Magnitude);
	}
}
