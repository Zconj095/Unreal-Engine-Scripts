// Copyright Epic Games, Inc.

#include "QAOAComponent.h"

#include "DynamicCircuitEditorComponent.h"
#include "Logging/LogMacros.h"

UQAOAComponent::UQAOAComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQAOAComponent::Execute(UDynamicCircuitEditorComponent* CircuitEditor)
{
	if (!CircuitEditor)
	{
		UE_LOG(LogTemp, Warning, TEXT("QAOA: CircuitEditor is null."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Executing QAOA..."));

	// Phase Separator gate (placeholder)
	FQuantumGate PhaseSeparator = FQuantumGate::CreateCNOT(0, 1);
	CircuitEditor->CircuitGates.Add(PhaseSeparator);

	// Mixer gate (placeholder: another CNOT for illustration)
	FQuantumGate Mixer = FQuantumGate::CreateCNOT(1, 0);
	CircuitEditor->CircuitGates.Add(Mixer);

	UE_LOG(LogTemp, Log, TEXT("QAOA executed successfully (placeholder gates added)."));
}
