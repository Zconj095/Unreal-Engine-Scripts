// Copyright Epic Games, Inc.

#include "QuantumFluxManagerComponent.h"

float UQuantumFluxManagerComponent::ComputeFlux()
{
	const float FluxOutput = Flux1 + Flux2 + Flux3;
	UE_LOG(LogTemp, Log, TEXT("Quantum Flux Output: %f"), FluxOutput);
	return FluxOutput;
}
