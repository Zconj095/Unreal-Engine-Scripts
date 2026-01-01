// Copyright Epic Games, Inc.

#include "QuantumBarebonesKernelCircuitComponent.h"

#include "Logging/LogMacros.h"

UQuantumBarebonesKernelCircuitComponent::UQuantumBarebonesKernelCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumBarebonesKernelCircuitComponent::ExecuteKernelCircuit(int32 NumQubits)
{
	UE_LOG(LogTemp, Log, TEXT("Executing Barebones Kernel Circuit with %d qubits..."), NumQubits);

	for (int32 i = 0; i < NumQubits; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Applying H Gate to Qubit %d..."), i);
		UE_LOG(LogTemp, Log, TEXT("Applying CNOT Gate from Qubit %d to Qubit %d..."), i, (i + 1) % FMath::Max(NumQubits, 1));
	}

	UE_LOG(LogTemp, Log, TEXT("Barebones Kernel Circuit executed."));
}
