// Copyright Epic Games, Inc.

#include "QuantumPhaseEstimationComponent.h"

FVector UQuantumPhaseEstimationComponent::EstimatePhase()
{
	UE_LOG(LogTemp, Log, TEXT("Estimating phase for state: %s"), *QuantumState.ToString());

	QuantumState = FVector(
		QuantumState.X * PI,
		QuantumState.Y * PI,
		QuantumState.Z * PI);

	UE_LOG(LogTemp, Log, TEXT("Phase Estimated State: %s"), *QuantumState.ToString());
	return QuantumState;
}
