// Copyright Epic Games, Inc.

#include "DynamicCircuitEditorComponent.h"

UDynamicCircuitEditorComponent::UDynamicCircuitEditorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDynamicCircuitEditorComponent::AddGate(const FString& GateType, int32 TargetQubit, int32 ControlQubit)
{
    FQuantumGate GateToAdd;
    bool bAdded = false;

    if (GateType.Equals(TEXT("Hadamard"), ESearchCase::IgnoreCase))
    {
        GateToAdd = FQuantumGate::CreateHadamard(TargetQubit);
        bAdded = true;
    }
    else if (GateType.Equals(TEXT("CNOT"), ESearchCase::IgnoreCase) && ControlQubit >= 0)
    {
        GateToAdd = FQuantumGate::CreateCNOT(ControlQubit, TargetQubit);
        bAdded = true;
    }

    if (bAdded)
    {
        CircuitGates.Add(GateToAdd);
        UE_LOG(LogTemp, Log, TEXT("Added %s gate to qubit %d%s."),
            *GateType,
            TargetQubit,
            GateType.Equals(TEXT("CNOT"), ESearchCase::IgnoreCase) ? *FString::Printf(TEXT(" (control %d)"), ControlQubit) : TEXT(""));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unsupported gate type or invalid control for %s."), *GateType);
    }
}

void UDynamicCircuitEditorComponent::RemoveGate(int32 GateIndex)
{
    if (CircuitGates.IsValidIndex(GateIndex))
    {
        CircuitGates.RemoveAt(GateIndex);
        UE_LOG(LogTemp, Log, TEXT("Removed gate at index %d."), GateIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid gate index %d."), GateIndex);
    }
}

void UDynamicCircuitEditorComponent::ReorderGate(int32 FromIndex, int32 ToIndex)
{
    if (CircuitGates.IsValidIndex(FromIndex) && CircuitGates.IsValidIndex(ToIndex))
    {
        const FQuantumGate Gate = CircuitGates[FromIndex];
        CircuitGates.RemoveAt(FromIndex);
        CircuitGates.Insert(Gate, ToIndex);
        UE_LOG(LogTemp, Log, TEXT("Reordered gate from index %d to index %d."), FromIndex, ToIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid gate indices for reordering (%d -> %d)."), FromIndex, ToIndex);
    }
}
