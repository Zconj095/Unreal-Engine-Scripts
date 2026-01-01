#include "NeurogenomicProcessingEfficiencyComponent.h"
#include "Logging/LogMacros.h"

UNeurogenomicProcessingEfficiencyComponent::UNeurogenomicProcessingEfficiencyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeurogenomicProcessingEfficiencyComponent::BeginPlay()
{
	Super::BeginPlay();

	ProcessingEfficiency = CalculateProcessingEfficiency();
	if (ProcessingEfficiency > 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Neurogenomic Processing Efficiency (?_n): %0.4f"), ProcessingEfficiency);
	}
}

float UNeurogenomicProcessingEfficiencyComponent::CalculateProcessingEfficiency() const
{
	if (Entropy <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Entropy (?S) must be greater than 0!"));
		return 0.0f;
	}

	return TotalInformation / Entropy;
}
