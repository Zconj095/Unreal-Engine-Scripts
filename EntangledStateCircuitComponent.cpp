// Copyright Epic Games, Inc.

#include "EntangledStateCircuitComponent.h"

UEntangledStateCircuitComponent::UEntangledStateCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEntangledStateCircuitComponent::CreateEntangledState(int32 NumQubits)
{
    UE_LOG(LogTemp, Log, TEXT("Creating entangled state circuit for %d qubits."), NumQubits);
}
