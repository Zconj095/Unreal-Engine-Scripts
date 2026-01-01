// Copyright Epic Games, Inc.

#include "HyperdimensionalQuantumCircuitComponent.h"

UHyperdimensionalQuantumCircuitComponent::UHyperdimensionalQuantumCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHyperdimensionalQuantumCircuitComponent::ApplyGate(const FString& Gate, const TArray<int32>& TargetQubits)
{
	TArray<FString> TargetStrings;
	for (int32 Qubit : TargetQubits)
	{
		TargetStrings.Add(FString::FromInt(Qubit));
	}

	const FString GateOperation = FString::Printf(TEXT("%s on Qubits: %s"), *Gate, *FString::Join(TargetStrings, TEXT(", ")));
	Gates.Add(GateOperation);
	UE_LOG(LogTemp, Log, TEXT("Applied Gate: %s"), *GateOperation);
}

void UHyperdimensionalQuantumCircuitComponent::PrintCircuit() const
{
	UE_LOG(LogTemp, Log, TEXT("Hyperdimensional Quantum Circuit:"));
	for (const FString& GateOp : Gates)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *GateOp);
	}
}
