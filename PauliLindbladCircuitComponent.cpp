// Copyright Epic Games, Inc.

#include "PauliLindbladCircuitComponent.h"

UPauliLindbladCircuitComponent::UPauliLindbladCircuitComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPauliLindbladCircuitComponent::ApplyPauliLindblad(FVector PauliVector)
{
    UE_LOG(LogTemp, Log, TEXT("Applying Pauli-Lindblad noise with vector (%f, %f, %f)."), PauliVector.X, PauliVector.Y, PauliVector.Z);
}
