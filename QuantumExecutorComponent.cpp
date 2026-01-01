// Copyright Epic Games, Inc.

#include "QuantumExecutorComponent.h"

#include "Logging/LogMacros.h"

UQuantumExecutorComponent::UQuantumExecutorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumExecutorComponent::ExecuteCircuit()
{
	UE_LOG(LogTemp, Log, TEXT("Executing quantum circuit with %d qubits."), NumQubits);

	for (const FString& Op : Operations)
	{
		UE_LOG(LogTemp, Log, TEXT("Executing operation: %s"), *Op);
		// Add operation-specific logic here.
	}

	UE_LOG(LogTemp, Log, TEXT("Quantum circuit execution completed."));
}

void UQuantumExecutorComponent::AddOperation(const FString& Operation)
{
	Operations.Add(Operation);
	UE_LOG(LogTemp, Log, TEXT("Added operation: %s"), *Operation);
}
