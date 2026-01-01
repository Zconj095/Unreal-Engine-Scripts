// Copyright Epic Games, Inc.

#include "ParallelCircuitSimulatorComponent.h"

#include "Async/ParallelFor.h"
#include "Logging/LogMacros.h"

UParallelCircuitSimulatorComponent::UParallelCircuitSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UParallelCircuitSimulatorComponent::SimulateCircuits(const TArray<TArray<FString>>& Circuits)
{
	if (Circuits.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No circuits provided to simulate."));
		return;
	}

	ParallelFor(Circuits.Num(), [this, &Circuits](int32 Index)
	{
		SimulateSingleCircuit(Circuits[Index], Index);
	});

	UE_LOG(LogTemp, Log, TEXT("All circuits simulated in parallel."));
}

void UParallelCircuitSimulatorComponent::SimulateSingleCircuit(const TArray<FString>& Circuit, int32 CircuitIndex) const
{
	UE_LOG(LogTemp, Log, TEXT("Simulating Circuit %d"), CircuitIndex);
	for (const FString& Gate : Circuit)
	{
		UE_LOG(LogTemp, Log, TEXT("Executing Gate: %s"), *Gate);
		// Add simulation logic here.
	}
}
