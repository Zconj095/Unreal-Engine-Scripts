#include "NeuralResonance2Component.h"
#include "Logging/LogMacros.h"

UNeuralResonance2Component::UNeuralResonance2Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralResonance2Component::BeginPlay()
{
	Super::BeginPlay();

	ResonanceFactor = CalculateNeuralResonance(Amplitude, Period, TimeAccumulator);
}

void UNeuralResonance2Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeAccumulator += DeltaTime;
	ResonanceFactor = CalculateNeuralResonance(Amplitude, Period, TimeAccumulator);
	UE_LOG(LogTemp, Log, TEXT("Neural Resonance (Rₙ): %0.4f"), ResonanceFactor);
}

float UNeuralResonance2Component::CalculateNeuralResonance(float A, float P, float t) const
{
	if (P <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Period (P) must be greater than zero."));
		return 0.0f;
	}

	return A * FMath::Cos((2.0f * PI * t) / P);
}

void UNeuralResonance2Component::SetAmplitude(float Value)
{
	Amplitude = Value;
}

void UNeuralResonance2Component::SetPeriod(float Value)
{
	Period = Value;
}

void UNeuralResonance2Component::ResetTime()
{
	TimeAccumulator = 0.0f;
}
