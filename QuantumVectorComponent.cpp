// Copyright Epic Games, Inc.

#include "QuantumVectorComponent.h"

UQuantumVectorComponent::UQuantumVectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UQuantumVectorComponent::Generate() const
{
	const FVector QuantumState = InputState;
	UE_LOG(LogTemp, Log, TEXT("Generating Quantum Vector: %s"), *QuantumState.ToString());
	return QuantumState;
}
