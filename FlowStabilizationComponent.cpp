#include "FlowStabilizationComponent.h"
#include "Math/UnrealMathUtility.h"

UFlowStabilizationComponent::UFlowStabilizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	VelocityGradient = 10.0;
	TimeGradient = 2.0;
	MagicalEnergyContribution = 5.0;
}

void UFlowStabilizationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (FMath::IsNearlyZero(TimeGradient))
	{
		UE_LOG(LogTemp, Error, TEXT("Time gradient cannot be zero."));
		return;
	}

	const double StabilizationIndex = (VelocityGradient / TimeGradient) + MagicalEnergyContribution;
	UE_LOG(LogTemp, Log, TEXT("Stabilization Index (S): %f"), StabilizationIndex);
}
