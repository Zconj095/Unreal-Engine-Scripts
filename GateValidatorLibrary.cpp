// Copyright Epic Games, Inc.

#include "GateValidatorLibrary.h"

#include "DynamicCircuitEditorComponent.h"

bool UGateValidatorLibrary::IsValidGate(const UDynamicCircuitEditorComponent* CircuitEditor, const FString& GateType, int32 TargetQubit, int32 ControlQubit)
{
	if (!CircuitEditor)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsValidGate: CircuitEditor is null."));
		return false;
	}

	const int32 NumQubits = CircuitEditor->NumQubits;

	if (TargetQubit < 0 || TargetQubit >= NumQubits)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid target qubit %d."), TargetQubit);
		return false;
	}

	if (GateType.Equals(TEXT("CNOT"), ESearchCase::IgnoreCase))
	{
		if (ControlQubit < 0 || ControlQubit >= NumQubits || ControlQubit == TargetQubit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid control qubit %d for CNOT gate."), ControlQubit);
			return false;
		}
	}

	// Add other gate-specific validations as needed.

	return true;
}
