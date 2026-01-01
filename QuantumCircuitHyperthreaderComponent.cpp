// Copyright Epic Games, Inc.

#include "QuantumCircuitHyperthreaderComponent.h"

#include "Async/ParallelFor.h"
#include "Logging/LogMacros.h"

UQuantumCircuitHyperthreaderComponent::UQuantumCircuitHyperthreaderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumCircuitHyperthreaderComponent::SimulateCircuit()
{
	if (CircuitGates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No gates to simulate."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Starting Quantum Circuit Simulation with Hyperthreading..."));

	ParallelFor(CircuitGates.Num(), [this](int32 Index)
	{
		SimulateGate(CircuitGates[Index], Index);
	});

	UE_LOG(LogTemp, Log, TEXT("Quantum Circuit Simulation Completed."));
}

void UQuantumCircuitHyperthreaderComponent::SimulateGate(const FString& Gate, int32 Index) const
{
	UE_LOG(LogTemp, Log, TEXT("Thread %d: Simulating Gate %s on Qubit %d"), FPlatformTLS::GetCurrentThreadId(), *Gate, Index);
	// Add gate application logic here.
}
