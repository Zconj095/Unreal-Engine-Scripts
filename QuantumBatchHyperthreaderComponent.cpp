// Copyright Epic Games, Inc.

#include "QuantumBatchHyperthreaderComponent.h"

#include "Async/ParallelFor.h"
#include "Logging/LogMacros.h"

UQuantumBatchHyperthreaderComponent::UQuantumBatchHyperthreaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumBatchHyperthreaderComponent::SimulateAllCircuits(const TArray<FQuantumCircuitBatchEntry>& QuantumCircuits)
{
	if (QuantumCircuits.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No circuits provided for batch simulation."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Starting Batch Quantum Circuit Simulation..."));

	ParallelFor(QuantumCircuits.Num(), [this, &QuantumCircuits](int32 Index)
	{
		SimulateSingleCircuit(QuantumCircuits[Index], Index);
	});

	UE_LOG(LogTemp, Log, TEXT("Batch Quantum Circuit Simulation Completed."));
}

void UQuantumBatchHyperthreaderComponent::SimulateSingleCircuit(const FQuantumCircuitBatchEntry& Circuit, int32 CircuitIndex) const
{
	UE_LOG(LogTemp, Log, TEXT("Simulating Circuit %d"), CircuitIndex);
	for (const FString& Gate : Circuit.Gates)
	{
		UE_LOG(LogTemp, Log, TEXT("Circuit %d: Simulating Gate %s"), CircuitIndex, *Gate);
		// Add gate logic here.
	}
}
