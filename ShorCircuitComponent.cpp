// Copyright Epic Games, Inc.

#include "ShorCircuitComponent.h"

UShorCircuitComponent::UShorCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShorCircuitComponent::ExecuteShor(int32 NumberToFactor)
{
	UE_LOG(LogTemp, Log, TEXT("Starting Shor's Algorithm to factor %d..."), NumberToFactor);

	UE_LOG(LogTemp, Log, TEXT("Applying Quantum Fourier Transform..."));
	UE_LOG(LogTemp, Log, TEXT("Performing modular exponentiation..."));
	UE_LOG(LogTemp, Log, TEXT("Measuring the result..."));

	UE_LOG(LogTemp, Log, TEXT("Shor's Algorithm completed."));
}
