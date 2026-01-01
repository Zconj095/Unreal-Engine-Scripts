#include "PhenologicalCyclePredictionComponent.h"
#include "Logging/LogMacros.h"

UPhenologicalCyclePredictionComponent::UPhenologicalCyclePredictionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPhenologicalCyclePredictionComponent::BeginPlay()
{
	Super::BeginPlay();

	CalculatePhenology();
}

void UPhenologicalCyclePredictionComponent::CalculatePhenology()
{
	if (DailyMaxTemps.Num() != DailyMinTemps.Num() || DailyMaxTemps.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Daily temperature arrays must have equal non-zero length."));
		return;
	}

	GrowingDegreeDays = 0.0f;
	for (int32 Index = 0; Index < DailyMaxTemps.Num(); ++Index)
	{
		const float AvgTemp = (DailyMaxTemps[Index] + DailyMinTemps[Index]) * 0.5f;
		GrowingDegreeDays += FMath::Max(0.0f, AvgTemp - BaseTemperature);
	}

	PredictedEvent = Alpha * GrowingDegreeDays + Beta;
	UE_LOG(LogTemp, Log, TEXT("Calculated GDD: %0.4f"), GrowingDegreeDays);
	UE_LOG(LogTemp, Log, TEXT("Predicted Phenological Event Timing (P): %0.4f"), PredictedEvent);
}
