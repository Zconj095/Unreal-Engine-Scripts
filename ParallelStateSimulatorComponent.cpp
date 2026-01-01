// Copyright Epic Games, Inc.

#include "ParallelStateSimulatorComponent.h"

#include "DynamicCircuitEditorComponent.h"
#include "OptimizedQuantumState.h"
#include "Async/ParallelFor.h"
#include "Logging/LogMacros.h"

UParallelStateSimulatorComponent::UParallelStateSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UParallelStateSimulatorComponent::ConfigureCircuit(UDynamicCircuitEditorComponent* InCircuitEditor, int32 NumQubits)
{
	CircuitEditor = InCircuitEditor;

	if (!State)
	{
		State = NewObject<UOptimizedQuantumState>(this);
	}

	if (State)
	{
		State->InitializeState(NumQubits);
	}
}

void UParallelStateSimulatorComponent::Simulate()
{
	if (!CircuitEditor || !State)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParallelStateSimulator: Missing circuit editor or state."));
		return;
	}

	const TArray<FQuantumGate>& Gates = CircuitEditor->CircuitGates;
	if (Gates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParallelStateSimulator: No gates to simulate."));
		return;
	}

	// Apply each gate in parallel. Each gate operation writes to a temp buffer and then we commit gate-by-gate.
	// Note: If gates commute/independent, this is fine; otherwise sequence matters and parallel here is illustrative.
	for (const FQuantumGate& Gate : Gates)
	{
		State->ApplyGate([&Gate](int32 Index, const TArray<FComplexNumber>& InputState, TArray<FComplexNumber>& OutputState)
		{
			// Placeholder: copy input to output (no-op). Replace with actual gate math per index.
			OutputState[Index] = InputState[Index];
		});
	}

	UE_LOG(LogTemp, Log, TEXT("Parallel simulation complete."));
}
