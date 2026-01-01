#include "NeuralResonanceComponent.h"
#include "Logging/LogMacros.h"

UNeuralResonanceComponent::UNeuralResonanceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralResonanceComponent::BeginPlay()
{
	Super::BeginPlay();

	ResonanceFactor = CalculateNeuralResonance(Amplitude, Period, TimeAccumulator);
}

void UNeuralResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeAccumulator += DeltaTime;
	ResonanceFactor = CalculateNeuralResonance(Amplitude, Period, TimeAccumulator);
	UE_LOG(LogTemp, Log, TEXT("Neural Resonance (Rₙ): %0.4f"), ResonanceFactor);
}

float UNeuralResonanceComponent::CalculateNeuralResonance(float A, float P, float t) const
{
	if (P <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Period (P) must be greater than zero."));
		return 0.0f;
	}

	return A * FMath::Cos((2.0f * PI * t) / P);
}

void UNeuralResonanceComponent::SetAmplitude(float Value)
{
	Amplitude = Value;
}

void UNeuralResonanceComponent::SetPeriod(float Value)
{
	Period = Value;
}

void UNeuralResonanceComponent::ResetTime()
{
	TimeAccumulator = 0.0f;
}
