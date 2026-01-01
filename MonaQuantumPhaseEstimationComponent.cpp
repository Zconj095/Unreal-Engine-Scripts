// Copyright Epic Games, Inc.

#include "MonaQuantumPhaseEstimationComponent.h"

FVector UMonaQuantumPhaseEstimationComponent::EstimatePhase() const
{
	const FVector Phase = QuantumState * PI;
	UE_LOG(LogTemp, Log, TEXT("Estimating Phase for State: %s"), *QuantumState.ToString());
	UE_LOG(LogTemp, Log, TEXT("Estimated Phase: %s"), *Phase.ToString());
	return Phase;
}
