#include "NeuralOscillationHarmonyComponent.h"

#include "Engine/World.h"

UNeuralOscillationHarmonyComponent::UNeuralOscillationHarmonyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralOscillationHarmonyComponent::BeginPlay()
{
	Super::BeginPlay();

	const float NOHValue = CalculateNOH();
	UE_LOG(LogTemp, Log, TEXT("Neural Oscillation Harmony (NOH): %.4f"), NOHValue);
}

float UNeuralOscillationHarmonyComponent::CalculateNOH() const
{
	const int32 WaveCount = Brainwaves.Num();
	if (WaveCount == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The list of brainwaves is empty. Add some brainwaves to calculate NOH."));
		return 0.0f;
	}

	double Sum = 0.0;
	for (const FBrainwaveDescriptor& Brainwave : Brainwaves)
	{
		Sum += Brainwave.Amplitude * Brainwave.Frequency;
	}

	return static_cast<float>(Sum / WaveCount);
}
