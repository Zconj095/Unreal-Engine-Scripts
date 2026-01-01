// Copyright Epic Games, Inc.

#include "QuantumFusionStateCircuitComponent.h"

#include "Logging/LogMacros.h"

UQuantumFusionStateCircuitComponent::UQuantumFusionStateCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumFusionStateCircuitComponent::CreateFusionState(int32 NumStates)
{
	UE_LOG(LogTemp, Log, TEXT("Fusing %d quantum states..."), NumStates);

	for (int32 i = 0; i < NumStates - 1; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Fusing State %d with State %d..."), i, i + 1);
	}

	UE_LOG(LogTemp, Log, TEXT("Quantum fusion state created."));
}
