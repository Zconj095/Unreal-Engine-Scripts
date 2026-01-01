// Copyright Epic Games, Inc.

#include "QuantumCircuitObject.h"

void UQuantumCircuitObject::Initialize(int32 InNumQubits)
{
	NumQubits = FMath::Max(0, InNumQubits);
	Gates.Reset();
}

void UQuantumCircuitObject::AddQubit()
{
	++NumQubits;
}

void UQuantumCircuitObject::AddGate(const FQuantumGate& Gate)
{
	Gates.Add(Gate);
}

FString UQuantumCircuitObject::ToString() const
{
	FString Result = FString::Printf(TEXT("Quantum Circuit with %d Qubits:\n"), NumQubits);
	for (const FQuantumGate& Gate : Gates)
	{
		Result += FString::Printf(TEXT("GateType=%d, PrimaryIndex=%d, SecondaryIndex=%d\n"),
			static_cast<int32>(Gate.GateType),
			Gate.PrimaryIndex,
			Gate.SecondaryIndex);
	}
	return Result;
}
