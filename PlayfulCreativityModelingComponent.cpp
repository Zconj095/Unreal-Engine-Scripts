#include "PlayfulCreativityModelingComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPlayfulCreativityModelingComponent::UPlayfulCreativityModelingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayfulCreativityModelingComponent::BeginPlay()
{
	Super::BeginPlay();

	CreativityIndex = CalculateCreativityIndex();
	if (CreativityIndex > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Creativity Index (C): %0.4f"), CreativityIndex);
	}
}

float UPlayfulCreativityModelingComponent::CalculateCreativityIndex() const
{
	if (FMath::IsNearlyZero(TimeMinutes) || TimeMinutes < TimeTolerance)
	{
		UE_LOG(LogTemp, Error, TEXT("Time (T) cannot be zero or near zero!"));
		return 0.0f;
	}

	return NovelIdeas / TimeMinutes;
}
