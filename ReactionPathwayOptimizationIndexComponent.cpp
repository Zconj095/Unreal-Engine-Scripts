#include "ReactionPathwayOptimizationIndexComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UReactionPathwayOptimizationIndexComponent::UReactionPathwayOptimizationIndexComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UReactionPathwayOptimizationIndexComponent::BeginPlay()
{
	Super::BeginPlay();

	RPOIResult = CalculateRPOI();
	UE_LOG(LogTemp, Log, TEXT("Reaction Pathway Optimization Index (RPOI): %0.2f"), RPOIResult);
}

float UReactionPathwayOptimizationIndexComponent::CalculateRPOI() const
{
	if (FMath::IsNearlyZero(DeltaTime))
	{
		UE_LOG(LogTemp, Warning, TEXT("DeltaTime must not be zero when calculating RPOI."));
		return 0.0f;
	}

	return Kappa * (DeltaPOpt / DeltaTime);
}
