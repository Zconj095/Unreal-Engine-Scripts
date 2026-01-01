// Copyright Epic Games, Inc.

#include "QuantumHypervectorCircuitComponent.h"

#include "Logging/LogMacros.h"

UQuantumHypervectorCircuitComponent::UQuantumHypervectorCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHypervectorCircuitComponent::CreateHypervectorCircuit(int32 Dimension)
{
	UE_LOG(LogTemp, Log, TEXT("Creating Quantum Hypervector Circuit with dimension %d..."), Dimension);

	for (int32 i = 0; i < Dimension; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Encoding dimension %d into quantum hypervector..."), i);
	}

	UE_LOG(LogTemp, Log, TEXT("Quantum Hypervector Circuit created."));
}
