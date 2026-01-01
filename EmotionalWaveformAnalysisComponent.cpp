#include "EmotionalWaveformAnalysisComponent.h"
#include "Math/UnrealMathUtility.h"

UEmotionalWaveformAnalysisComponent::UEmotionalWaveformAnalysisComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	AmplitudeA = 1.0f;
	AmplitudeB = 0.5f;
	Frequency = 1.0f;
	ElapsedTime = 0.0f;
	EmotionalState = 0.0f;
}

void UEmotionalWaveformAnalysisComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEmotionalWaveformAnalysisComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;
	EmotionalState = CalculateEmotionalState(AmplitudeA, AmplitudeB, Frequency, ElapsedTime);

	UE_LOG(LogTemp, Log, TEXT("Emotional State (E(t)) at time %.2f: %f"), ElapsedTime, EmotionalState);
}

float UEmotionalWaveformAnalysisComponent::CalculateEmotionalState(float A, float B, float Omega, float Time) const
{
	const float AngularFrequency = 2.0f * PI * Omega;
	return A * FMath::Cos(AngularFrequency * Time) + B * FMath::Sin(AngularFrequency * Time);
}
