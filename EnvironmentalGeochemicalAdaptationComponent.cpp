#include "EnvironmentalGeochemicalAdaptationComponent.h"
#include "Math/UnrealMathUtility.h"

UEnvironmentalGeochemicalAdaptationComponent::UEnvironmentalGeochemicalAdaptationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Eta = 2.0f;
	DeltaGEnv = 5.0f;
	DeltaTime = 1.5f;
	EGAI = 0.0f;
}

void UEnvironmentalGeochemicalAdaptationComponent::BeginPlay()
{
	Super::BeginPlay();

	EGAI = CalculateEGAI(Eta, DeltaGEnv, DeltaTime);
	UE_LOG(LogTemp, Log, TEXT("Environmental Geochemical Adaptation Index (EGAI): %f"), EGAI);
}

float UEnvironmentalGeochemicalAdaptationComponent::CalculateEGAI(float InEta, float InDeltaGEnv, float InDeltaTime) const
{
	if (FMath::IsNearlyZero(InDeltaTime))
	{
		UE_LOG(LogTemp, Error, TEXT("Time interval (Delta t) cannot be zero!"));
		return 0.0f;
	}

	return InEta * (InDeltaGEnv / InDeltaTime);
}
