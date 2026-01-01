#include "HarmonicResonanceModelActor.h"

#include "Math/UnrealMathUtility.h"

AHarmonicResonanceModelActor::AHarmonicResonanceModelActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHarmonicResonanceModelActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ResonanceValue = ComputeHarmonicResonance(SymmetryOrder);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Harmonic Resonance (R_h): %.6f"), ResonanceValue);
	}
}

double AHarmonicResonanceModelActor::ComputeHarmonicResonance(float K) const
{
	const double Lower = 0.0;
	const double Upper = 2.0 * PI;
	const double Step = (Upper - Lower) / IntegrationResolution;
	double Sum = 0.0;

	for (int32 i = 0; i < IntegrationResolution; ++i)
	{
		const double Theta = Lower + (i + 0.5) * Step;
		Sum += FMath::Cos(K * Theta);
	}

	return Sum * Step;
}

double AHarmonicResonanceModelActor::GetResonance() const
{
	return ResonanceValue;
}
