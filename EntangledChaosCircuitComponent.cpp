// Copyright Epic Games, Inc.

#include "EntangledChaosCircuitComponent.h"

UEntangledChaosCircuitComponent::UEntangledChaosCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEntangledChaosCircuitComponent::CreateEntangledChaos(int32 NumQubits)
{
    UE_LOG(LogTemp, Log, TEXT("Creating entangled chaos circuit for %d qubits."), NumQubits);
}
