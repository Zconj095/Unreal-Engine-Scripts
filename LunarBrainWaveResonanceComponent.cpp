#include "LunarBrainWaveResonanceComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarBrainWaveResonanceComponent::ULunarBrainWaveResonanceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarBrainWaveResonanceComponent::BeginPlay()
{
	Super::BeginPlay();
	ResonanceFrequency = CalculateResonanceFrequency(0.0f);
	LogResonance(ResonanceFrequency);
}

void ULunarBrainWaveResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	ResonanceFrequency = CalculateResonanceFrequency(CurrentTime);
}

float ULunarBrainWaveResonanceComponent::CalculateResonanceFrequency(float Time) const
{
	return BaselineFrequency + FrequencyRange * FMath::Cos(AngularFrequency * Time + PhaseOffset);
}

void ULunarBrainWaveResonanceComponent::LogResonance(float Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Lunar Brain Wave Resonance Frequency: %.2f"), Value);
}
