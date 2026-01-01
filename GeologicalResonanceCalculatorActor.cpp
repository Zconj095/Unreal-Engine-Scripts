#include "GeologicalResonanceCalculatorActor.h"

#include "Math/UnrealMathUtility.h"

AGeologicalResonanceCalculatorActor::AGeologicalResonanceCalculatorActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeologicalResonanceCalculatorActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AccumulatedTime += DeltaSeconds * TimeStepScale;
	CurrentResonance = CalculateResonance(AccumulatedTime);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Time: %.2f, Geological Resonance Factor (R_g): %.4f"), AccumulatedTime, CurrentResonance);
	}
}

float AGeologicalResonanceCalculatorActor::CalculateResonance(float TimeValue) const
{
	return Amplitude * FMath::Cos((2.0f * PI * TimeValue) / Period);
}
