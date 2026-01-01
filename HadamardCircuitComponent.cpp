// Copyright Epic Games, Inc.

#include "HadamardCircuitComponent.h"

UHadamardCircuitComponent::UHadamardCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHadamardCircuitComponent::ApplyHadamard(int32 NumQubits)
{
    UE_LOG(LogTemp, Log, TEXT("Applying Hadamard circuit to %d qubits."), NumQubits);
}
