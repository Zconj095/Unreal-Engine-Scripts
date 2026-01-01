// Copyright Epic Games, Inc.

#include "CircuitAnalysisPassComponent.h"

UCircuitAnalysisPassComponent::UCircuitAnalysisPassComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCircuitAnalysisPassComponent::AnalyzeCircuit()
{
    TMap<FString, int32> GateCounts;

    for (const FGateDefinition& Gate : QuantumCircuit)
    {
        int32& Count = GateCounts.FindOrAdd(Gate.Type);
        ++Count;
    }

    const int32 CircuitDepth = CalculateCircuitDepth();

    UE_LOG(LogTemp, Log, TEXT("Circuit Analysis:"));
    for (const TPair<FString, int32>& Entry : GateCounts)
    {
        UE_LOG(LogTemp, Log, TEXT("Gate: %s, Count: %d"), *Entry.Key, Entry.Value);
    }
    UE_LOG(LogTemp, Log, TEXT("Circuit Depth: %d"), CircuitDepth);
}

int32 UCircuitAnalysisPassComponent::CalculateCircuitDepth() const
{
    // Simple model: each gate contributes a depth of 1.
    return QuantumCircuit.Num();
}

void UCircuitAnalysisPassComponent::AddGate(const FString& Type, const TArray<FString>& Qubits)
{
    QuantumCircuit.Add(FGateDefinition(Type, Qubits));
}
