#include "NeuralResonance4Component.h"
#include "Logging/LogMacros.h"

UNeuralResonance4Component::UNeuralResonance4Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralResonance4Component::BeginPlay()
{
	Super::BeginPlay();

	ResonanceFactor = CalculateResonance(Amplitude, Period, TimeElapsed);
}

void UNeuralResonance4Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeElapsed += DeltaTime;
	ResonanceFactor = CalculateResonance(Amplitude, Period, TimeElapsed);
	UE_LOG(LogTemp, Log, TEXT("Time: %0.2fs, Resonance Factor (Rb): %0.2f"), TimeElapsed, ResonanceFactor);
}

float UNeuralResonance4Component::CalculateResonance(float InAmplitude, float InPeriod, float InTime) const
{
	if (FMath::IsNearlyZero(InPeriod))
	{
		UE_LOG(LogTemp, Warning, TEXT("Period (P) cannot be zero. Returning 0."));
		return 0.0f;
	}

	return InAmplitude * FMath::Cos((2.0f * PI * InTime) / InPeriod);
}
