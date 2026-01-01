#include "GeoevolutionaryPatternPotentialActor.h"

#include "Math/UnrealMathUtility.h"

AGeoevolutionaryPatternPotentialActor::AGeoevolutionaryPatternPotentialActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeoevolutionaryPatternPotentialActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (XEnd <= XStart || Steps <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid interval or step count for GPP calculation."));
		return;
	}

	PatternPotential = CalculateGPP(XStart, XEnd, Steps, Time);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Geoevolutionary Pattern Potential (GPP): %.4f"), PatternPotential);
	}
}

float AGeoevolutionaryPatternPotentialActor::CalculateGPP(float Start, float End, int32 StepCount, float TimeValue) const
{
	const float DX = (End - Start) / StepCount;
	float Integral = 0.0f;
	for (int32 Index = 0; Index <= StepCount; ++Index)
	{
		const float X = Start + Index * DX;
		const float Wave = WaveFunction(X, TimeValue);
		const float Conjugate = ConjugateWaveFunction(X, TimeValue);
		Integral += Conjugate * Wave * DX;
	}
	return Integral;
}

float AGeoevolutionaryPatternPotentialActor::WaveFunction(float X, float TimeValue) const
{
	return FMath::Sin(X + TimeValue);
}

float AGeoevolutionaryPatternPotentialActor::ConjugateWaveFunction(float X, float TimeValue) const
{
	return FMath::Sin(X + TimeValue);
}
