#include "NeuralResonance5Component.h"
#include "Logging/LogMacros.h"

UNeuralResonance5Component::UNeuralResonance5Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralResonance5Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	ResonanceFactor = CalculateResonanceFactor(CurrentTime);
	UE_LOG(LogTemp, Log, TEXT("Neural Resonance Factor (Rn): %0.2f"), ResonanceFactor);
}

float UNeuralResonance5Component::CalculateResonanceFactor(float Time) const
{
	if (FMath::IsNearlyZero(Period))
	{
		return 0.0f;
	}

	return Amplitude * FMath::Cos(2.0f * PI * Time / Period);
}
