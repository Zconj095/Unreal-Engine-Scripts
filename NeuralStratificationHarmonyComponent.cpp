#include "NeuralStratificationHarmonyComponent.h"
#include "Logging/LogMacros.h"

UNeuralStratificationHarmonyComponent::UNeuralStratificationHarmonyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNeuralStratificationHarmonyComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateNSH();
}

void UNeuralStratificationHarmonyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RecalculateNSH();
	UE_LOG(LogTemp, Log, TEXT("NSH: %0.4f"), NSHValue);
}

void UNeuralStratificationHarmonyComponent::UpdateLayerStrengths(const TArray<float>& NewLayerStrengths)
{
	LayerStrengths = NewLayerStrengths;
	RecalculateNSH();
}

void UNeuralStratificationHarmonyComponent::UpdateLayerFrequencies(const TArray<float>& NewLayerFrequencies)
{
	LayerFrequencies = NewLayerFrequencies;
	RecalculateNSH();
}

float UNeuralStratificationHarmonyComponent::GetNSH() const
{
	return NSHValue;
}

void UNeuralStratificationHarmonyComponent::RecalculateNSH()
{
	if (LayerStrengths.Num() != LayerFrequencies.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Layer strengths and frequencies must have equal length."));
		NSHValue = 0.0f;
		return;
	}

	const int32 LayerCount = LayerStrengths.Num();
	if (LayerCount == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No active layers available for NSH calculation."));
		NSHValue = 0.0f;
		return;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < LayerCount; ++Index)
	{
		Sum += LayerStrengths[Index] * LayerFrequencies[Index];
	}

	NSHValue = Sum / static_cast<float>(LayerCount);
}
