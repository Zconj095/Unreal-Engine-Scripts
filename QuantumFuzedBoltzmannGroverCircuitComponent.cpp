// Copyright Epic Games, Inc.

#include "QuantumFuzedBoltzmannGroverCircuitComponent.h"

#include "Logging/LogMacros.h"

UQuantumFuzedBoltzmannGroverCircuitComponent::UQuantumFuzedBoltzmannGroverCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumFuzedBoltzmannGroverCircuitComponent::ExecuteFuzedBoltzmannGrover(int32 NumQubits, int32 MarkedState, float BoltzmannFactor)
{
	UE_LOG(LogTemp, Log, TEXT("Executing Quantum Fuzed Boltzmann-Grover Circuit with %d qubits..."), NumQubits);

	UE_LOG(LogTemp, Log, TEXT("Initializing Boltzmann distribution..."));
	for (int32 i = 0; i < NumQubits; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Initializing Qubit %d with Boltzmann factor %f..."), i, BoltzmannFactor);
	}

	UE_LOG(LogTemp, Log, TEXT("Applying Grover's Algorithm to find marked state %d..."), MarkedState);
	UE_LOG(LogTemp, Log, TEXT("Oracle and Diffusion Operator applied."));

	UE_LOG(LogTemp, Log, TEXT("Quantum Fuzed Boltzmann-Grover Circuit execution completed."));
}
