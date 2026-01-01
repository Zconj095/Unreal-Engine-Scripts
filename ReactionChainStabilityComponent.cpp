#include "ReactionChainStabilityComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UReactionChainStabilityComponent::UReactionChainStabilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UReactionChainStabilityComponent::BeginPlay()
{
	Super::BeginPlay();

	StabilityIndex = CalculateStabilityIndex();
	UE_LOG(LogTemp, Log, TEXT("Stability Index: %0.4f"), StabilityIndex);
}

float UReactionChainStabilityComponent::CalculateStabilityIndex() const
{
	const float Denominator = 1.0f + Alpha * ReactionRate * EnergyGainFactor;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid denominator when computing stability index."));
		return 0.0f;
	}

	return 1.0f / Denominator;
}
