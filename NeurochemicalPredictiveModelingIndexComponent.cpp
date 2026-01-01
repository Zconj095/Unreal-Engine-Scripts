#include "NeurochemicalPredictiveModelingIndexComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeurochemicalPredictiveModelingIndexComponent::UNeurochemicalPredictiveModelingIndexComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurochemicalPredictiveModelingIndexComponent::BeginPlay()
{
	Super::BeginPlay();

	NeurochemicalPredictiveModelingIndex = CalculateNPMI();
	UE_LOG(LogTemp, Log, TEXT("Neurochemical Predictive Modeling Index (NPMI): %0.4f"), NeurochemicalPredictiveModelingIndex);
}

float UNeurochemicalPredictiveModelingIndexComponent::CalculateNPMI() const
{
	if (FMath::IsNearlyZero(PredictionTime))
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Time interval for prediction (T) cannot be zero!"));
		return 0.0f;
	}

	return Eta * (DeltaPNeuro / PredictionTime);
}
