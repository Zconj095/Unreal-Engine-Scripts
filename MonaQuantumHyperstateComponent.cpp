// Copyright Epic Games, Inc.

#include "MonaQuantumHyperstateComponent.h"

FVector UMonaQuantumHyperstateComponent::FormHyperstate() const
{
	const FVector Hyperstate = (Input1 + Input2) * 0.5f;
	UE_LOG(LogTemp, Log, TEXT("Forming Quantum Hyperstate from Input1: %s, Input2: %s"), *Input1.ToString(), *Input2.ToString());
	UE_LOG(LogTemp, Log, TEXT("Quantum Hyperstate: %s"), *Hyperstate.ToString());
	return Hyperstate;
}
