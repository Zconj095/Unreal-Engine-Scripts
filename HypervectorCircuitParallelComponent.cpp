// Copyright Epic Games, Inc.

#include "HypervectorCircuitParallelComponent.h"

#include "Async/ParallelFor.h"

UHypervectorCircuitParallelComponent::UHypervectorCircuitParallelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHypervectorCircuitParallelComponent::ExecuteHypervectorCircuit()
{
	if (Dimension <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hypervector dimension must be positive."));
		return;
	}

	HypervectorValues.SetNum(Dimension);

	ParallelFor(Dimension, [this](int32 Index)
	{
		const float AngleRadians = FMath::DegreesToRadians(static_cast<float>(Index));
		HypervectorValues[Index] = FMath::Sin(AngleRadians); // Example computation
	});

	UE_LOG(LogTemp, Log, TEXT("Hypervector Circuit execution completed in parallel (Dimension=%d)."), Dimension);
}
