#include "RhythmPredictorComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URhythmPredictorComponent::URhythmPredictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InputTimes = {6.0f, 12.0f, 18.0f};
	FrequencyScale = 0.1f;
}

void URhythmPredictorComponent::BeginPlay()
{
	Super::BeginPlay();
	SimulateRhythm();
}

float URhythmPredictorComponent::PredictState(float Time) const
{
	const float Phase = (Time * FrequencyScale + Offset);
	return FMath::Sin(Phase);
}

void URhythmPredictorComponent::SimulateRhythm()
{
	PredictedStates.Empty();
	for (int32 Index = 0; Index < InputTimes.Num(); ++Index)
	{
		const float Time = InputTimes[Index];
		const float State = PredictState(Time);
		PredictedStates.Add(State);
		UE_LOG(LogTemp, Log, TEXT("Rhythm prediction at t=%.2f => %.4f"), Time, State);
	}
	const float DefaultState = PredictState(18.0f);
	UE_LOG(LogTemp, Log, TEXT("Default prediction at t=18.0 => %.4f"), DefaultState);
}
