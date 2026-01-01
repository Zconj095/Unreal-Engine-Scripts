// Copyright Epic Games, Inc.

#include "ChaosStateCircuitComponent.h"

UChaosStateCircuitComponent::UChaosStateCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UChaosStateCircuitComponent::CreateChaosState(int32 NumQubits)
{
    UE_LOG(LogTemp, Log, TEXT("Creating chaos state circuit for %d qubits."), NumQubits);
}
