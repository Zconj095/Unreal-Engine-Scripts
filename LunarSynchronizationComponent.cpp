#include "LunarSynchronizationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarSynchronizationComponent::ULunarSynchronizationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarSynchronizationComponent::BeginPlay()
{
	Super::BeginPlay();
	ElapsedTime = 0.0f;
}

void ULunarSynchronizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ElapsedTime >= SimulationDuration)
	{
		return;
	}

	const float Potential = CalculateLunarResonance(ElapsedTime);
	LogPotential(Potential);
	ElapsedTime += TimeStep;
}

float ULunarSynchronizationComponent::CalculateLunarResonance(float Time) const
{
	const float LunarWave = ScalingFactor * FMath::Cos(2.0f * PI * LunarFrequency * Time + PhaseShift);
	const float MentalWave = CalculateMentalFieldWave(Time);
	return LunarWave * MentalWave;
}

float ULunarSynchronizationComponent::CalculateMentalFieldWave(float Time) const
{
	const float X = FMath::Sin(Time);
	return MentalFieldAmplitude * FMath::Sin(2.0f * PI * MentalFieldFrequency * Time) * X;
}

void ULunarSynchronizationComponent::LogPotential(float Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Time: %.2fs, Lunar Potential: %.2f"), ElapsedTime, Value);
}
