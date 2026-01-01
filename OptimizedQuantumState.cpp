// Copyright Epic Games, Inc.

#include "OptimizedQuantumState.h"

#include "Async/ParallelFor.h"
#include "Logging/LogMacros.h"

void UOptimizedQuantumState::InitializeState(int32 InNumQubits)
{
	NumQubits = FMath::Max(0, InNumQubits);
	const int32 StateSize = (NumQubits > 0) ? (1 << NumQubits) : 1;

	StateVector.SetNum(StateSize);
	for (FComplexNumber& Amp : StateVector)
	{
		Amp.Real = 0.0;
		Amp.Imaginary = 0.0;
	}

	if (StateVector.Num() > 0)
	{
		StateVector[0].Real = 1.0; // |0...0>
	}
}

void UOptimizedQuantumState::ApplyGate(const TFunctionRef<void(int32, const TArray<FComplexNumber>&, TArray<FComplexNumber>&)>& Operation)
{
	if (StateVector.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyGate called on empty state vector."));
		return;
	}

	TArray<FComplexNumber> OutputState;
	OutputState.SetNum(StateVector.Num());

	ParallelFor(StateVector.Num(), [this, &Operation, &OutputState](int32 Index)
	{
		Operation(Index, StateVector, OutputState);
	});

	StateVector = MoveTemp(OutputState);
}
