#include "FeedbackStabilityComponent.h"
#include "Math/UnrealMathUtility.h"

UFeedbackStabilityComponent::UFeedbackStabilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Beta = 0.5f;
	GainG = 2.0f;
	TransferH = 1.5f;
}

void UFeedbackStabilityComponent::BeginPlay()
{
	Super::BeginPlay();

	const float Denominator = 1.0f + Beta * GainG * TransferH;
	if (FMath::IsNearlyZero(Denominator))
	{
		UE_LOG(LogTemp, Error, TEXT("Denominator is zero, stability index cannot be calculated!"));
		return;
	}

	const float StabilityIndex = 1.0f / Denominator;
	UE_LOG(LogTemp, Log, TEXT("Stability Index (S): %.4f"), StabilityIndex);
}
