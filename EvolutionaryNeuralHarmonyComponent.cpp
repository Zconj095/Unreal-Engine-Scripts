#include "EvolutionaryNeuralHarmonyComponent.h"
#include "Math/UnrealMathUtility.h"

UEvolutionaryNeuralHarmonyComponent::UEvolutionaryNeuralHarmonyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ActivationAmplitudes = {1.0f, 0.8f, 0.6f};
	SynchronizationFrequencies = {1.2f, 1.1f, 0.9f};
	ENH = 0.0f;
}

void UEvolutionaryNeuralHarmonyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEvolutionaryNeuralHarmonyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActivationAmplitudes.Num() != SynchronizationFrequencies.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Activation amplitudes and synchronization frequencies must be the same length."));
		ENH = 0.0f;
		return;
	}

	ENH = CalculateENH();
	UE_LOG(LogTemp, Log, TEXT("ENH: %f"), ENH);
}

float UEvolutionaryNeuralHarmonyComponent::CalculateENH() const
{
	const int32 Count = ActivationAmplitudes.Num();
	if (Count == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No neural clusters available for calculation."));
		return 0.0f;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < Count; ++Index)
	{
		Sum += ActivationAmplitudes[Index] * SynchronizationFrequencies[Index];
	}

	return Sum / FMath::Sqrt(static_cast<float>(Count));
}
