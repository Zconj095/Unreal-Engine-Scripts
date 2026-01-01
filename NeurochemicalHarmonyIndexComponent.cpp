#include "NeurochemicalHarmonyIndexComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurochemicalHarmonyIndexComponent::UNeurochemicalHarmonyIndexComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurochemicalHarmonyIndexComponent::BeginPlay()
{
	Super::BeginPlay();

	NeurochemicalHarmonyIndex = CalculateNHI();
	UE_LOG(LogTemp, Log, TEXT("Neurochemical Harmony Index (NHI): %0.4f"), NeurochemicalHarmonyIndex);
}

float UNeurochemicalHarmonyIndexComponent::CalculateNHI() const
{
	if (InteractionStrengths.Num() != SynchronizationFrequencies.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction strengths and synchronization frequencies must match in length."));
		return 0.0f;
	}

	const int32 ClusterCount = InteractionStrengths.Num();
	if (ClusterCount == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Cluster count must be greater than zero."));
		return 0.0f;
	}

	float Numerator = 0.0f;
	for (int32 Index = 0; Index < ClusterCount; ++Index)
	{
		Numerator += InteractionStrengths[Index] * SynchronizationFrequencies[Index];
	}

	const float Denominator = FMath::Sqrt(static_cast<float>(ClusterCount));
	return Denominator > 0.0f ? Numerator / Denominator : 0.0f;
}
