// Copyright Epic Games, Inc.

#include "ParticleStateCircuitComponent.h"

UParticleStateCircuitComponent::UParticleStateCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UParticleStateCircuitComponent::CreateParticleState(int32 NumQubits, FVector InitialPosition)
{
    UE_LOG(LogTemp, Log, TEXT("Creating particle state circuit for %d qubits at position %s."), NumQubits, *InitialPosition.ToString());
}
