// Copyright Epic Games, Inc.

#include "QuantumFrameworkManagerComponent.h"

#include "QuantumCircuitObject.h"
#include "Logging/LogMacros.h"

UQuantumFrameworkManagerComponent::UQuantumFrameworkManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumFrameworkManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("QuantumFrameworkManager: Starting..."));

	Circuit = NewObject<UQuantumCircuitObject>(this);
	if (Circuit)
	{
		Circuit->Initialize(NumQubits);
		UE_LOG(LogTemp, Log, TEXT("QuantumFrameworkManager: QuantumCircuit initialized successfully with %d qubits."), NumQubits);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumFrameworkManager: Failed to create QuantumCircuitObject."));
	}
}
