#include "RecombinationPredictorComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

URecombinationPredictorComponent::URecombinationPredictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DistanceSamples = {100.0f, 120.0f, 130.0f};
}

void URecombinationPredictorComponent::BeginPlay()
{
	Super::BeginPlay();
	SimulateProbabilities();
}

void URecombinationPredictorComponent::SimulateProbabilities()
{
	PredictedProbabilities.Empty();
	for (int32 Index = 0; Index < DistanceSamples.Num(); ++Index)
	{
		const float Distance = DistanceSamples[Index];
		const float Probability = PredictProbability(Distance);
		PredictedProbabilities.Add(Probability);
		UE_LOG(LogTemp, Log, TEXT("Probability [%d]: %0.4f"), Index + 1, Probability);
	}
}

float URecombinationPredictorComponent::PredictProbability(float Distance) const
{
	if (Distance <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Distance must be positive for probability prediction."));
		return 0.0f;
	}

	const float Base = 1.0f - FMath::Exp(-Beta * Distance);
	return FMath::Clamp(Base + MagicalFieldInfluence, 0.0f, 1.0f);
}
