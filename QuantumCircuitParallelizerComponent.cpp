// Copyright Epic Games, Inc.

#include "QuantumCircuitParallelizerComponent.h"

#include "Async/ParallelFor.h"
#include "Logging/LogMacros.h"

UQuantumCircuitParallelizerComponent::UQuantumCircuitParallelizerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumCircuitParallelizerComponent::ApplyGatesParallel(const TArray<FString>& Gates, const TArray<int32>& TargetQubits)
{
	if (Gates.Num() != TargetQubits.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Mismatch between gates and target qubits."));
		return;
	}

	ParallelFor(Gates.Num(), [this, &Gates, &TargetQubits](int32 Index)
	{
		ApplyGate(Gates[Index], TargetQubits[Index]);
	});

	UE_LOG(LogTemp, Log, TEXT("Parallel gate application completed."));
}

void UQuantumCircuitParallelizerComponent::ApplyGate(const FString& Gate, int32 Qubit) const
{
	UE_LOG(LogTemp, Log, TEXT("Applied %s on Qubit %d"), *Gate, Qubit);
	// Add real gate logic here (e.g., matrix multiplication or state modification).
}
