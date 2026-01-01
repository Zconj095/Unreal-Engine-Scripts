// Copyright Epic Games, Inc.

#include "QuantumGatePalletComponent.h"

UQuantumGatePalletComponent::UQuantumGatePalletComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FString UQuantumGatePalletComponent::GetGate(int32 Index) const
{
	if (!GateTypes.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid gate index."));
		return FString();
	}
	return GateTypes[Index];
}

FString UQuantumGatePalletComponent::ListAvailableGates() const
{
	return FString::Join(GateTypes, TEXT(", "));
}
