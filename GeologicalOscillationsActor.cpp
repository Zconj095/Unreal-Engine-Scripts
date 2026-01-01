#include "GeologicalOscillationsActor.h"

#include "Math/UnrealMathUtility.h"

AGeologicalOscillationsActor::AGeologicalOscillationsActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeologicalOscillationsActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AccumulatedTime += DeltaSeconds * TimeStepScale;
	NeuralOscillation = CalculateOscillation(AccumulatedTime);
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("Time: %.2f, Neural Oscillation (S(t)): %.4f"), AccumulatedTime, NeuralOscillation);
	}
}

float AGeologicalOscillationsActor::CalculateOscillation(float TimeSeconds) const
{
	return Amplitude * FMath::Sin((2.0f * PI * Frequency * TimeSeconds) + PhaseOffset);
}
