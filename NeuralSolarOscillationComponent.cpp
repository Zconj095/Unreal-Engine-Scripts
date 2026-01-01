#include "NeuralSolarOscillationComponent.h"
#include "Logging/LogMacros.h"

UNeuralSolarOscillationComponent::UNeuralSolarOscillationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralSolarOscillationComponent::BeginPlay()
{
	Super::BeginPlay();

	NeuralOscillation = CalculateNSO(CurrentTime);
}

void UNeuralSolarOscillationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime += DeltaTime;
	NeuralOscillation = CalculateNSO(CurrentTime);
	UE_LOG(LogTemp, Log, TEXT("Time: %0.2fs, Neural Solar Oscillation (S(t)): %0.2f"), CurrentTime, NeuralOscillation);
}

float UNeuralSolarOscillationComponent::CalculateNSO(float Time) const
{
	return Amplitude * FMath::Sin(Frequency * Time + PhaseOffset);
}
