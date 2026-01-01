#include "EnvironmentalSystemAnalysisComponent.h"
#include "Math/UnrealMathUtility.h"

UEnvironmentalSystemAnalysisComponent::UEnvironmentalSystemAnalysisComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Weights = {0.5f, 0.3f, 0.2f};
	Intensities = {100.0f, 200.0f, 150.0f};
	SystemStrength = 0.0f;
}

void UEnvironmentalSystemAnalysisComponent::BeginPlay()
{
	Super::BeginPlay();

	SystemStrength = CalculateSystemStrength(Weights, Intensities);
	UE_LOG(LogTemp, Log, TEXT("Total System Interaction Strength (S): %.2f"), SystemStrength);
}

float UEnvironmentalSystemAnalysisComponent::CalculateSystemStrength(const TArray<float>& InWeights, const TArray<float>& InIntensities) const
{
	if (InWeights.Num() != InIntensities.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Weights and intensities must have the same length."));
		return 0.0f;
	}

	float Total = 0.0f;
	for (int32 Index = 0; Index < InWeights.Num(); ++Index)
	{
		Total += InWeights[Index] * InIntensities[Index];
	}
	return Total;
}
