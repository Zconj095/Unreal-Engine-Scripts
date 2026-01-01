// Copyright Epic Games, Inc.

#include "QuantumExperimentParallelizerComponent.h"

#include "Async/ParallelFor.h"
#include "Logging/LogMacros.h"

UQuantumExperimentParallelizerComponent::UQuantumExperimentParallelizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumExperimentParallelizerComponent::RunExperiments(const TArray<FExperimentConfiguration>& ExperimentConfigurations)
{
	if (ExperimentConfigurations.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No experiment configurations provided."));
		return;
	}

	ParallelFor(ExperimentConfigurations.Num(), [this, &ExperimentConfigurations](int32 Index)
	{
		ExecuteExperiment(ExperimentConfigurations[Index], Index);
	});

	UE_LOG(LogTemp, Log, TEXT("All experiments executed in parallel."));
}

void UQuantumExperimentParallelizerComponent::ExecuteExperiment(const FExperimentConfiguration& Config, int32 ExperimentIndex) const
{
	UE_LOG(LogTemp, Log, TEXT("Running Experiment %d"), ExperimentIndex);
	for (const FString& Step : Config.Steps)
	{
		UE_LOG(LogTemp, Log, TEXT("Step: %s"), *Step);
		// Add experiment logic here.
	}
}
