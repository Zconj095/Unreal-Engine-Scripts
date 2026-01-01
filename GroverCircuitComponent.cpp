// Copyright Epic Games, Inc.

#include "GroverCircuitComponent.h"

UGroverCircuitComponent::UGroverCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGroverCircuitComponent::ExecuteGrover(int32 NumQubits, int32 TargetIndex)
{
    UE_LOG(LogTemp, Log, TEXT("Executing Grover's algorithm on %d qubits to find target %d."), NumQubits, TargetIndex);
}
