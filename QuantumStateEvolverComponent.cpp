// Copyright Epic Games, Inc.

#include "QuantumStateEvolverComponent.h"

#include "Async/ParallelFor.h"

UQuantumStateEvolverComponent::UQuantumStateEvolverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumStateEvolverComponent::EvolveState()
{
	UE_LOG(LogTemp, Log, TEXT("Starting Quantum State Evolution..."));

	const int32 ClampedIterations = FMath::Max(0, Iterations);
	ParallelFor(ClampedIterations, [this](int32 Index)
	{
		SimulateStateEvolution(Index);
	});

	UE_LOG(LogTemp, Log, TEXT("Quantum State Evolution Completed."));
}

void UQuantumStateEvolverComponent::SimulateStateEvolution(int32 Iteration)
{
	const uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
	UE_LOG(LogTemp, Log, TEXT("Thread %u: Evolving state for iteration %d"), ThreadId, Iteration);
	// Placeholder: add actual quantum state computation here.
}
