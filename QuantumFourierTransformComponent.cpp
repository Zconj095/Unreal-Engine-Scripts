// Copyright Epic Games, Inc.

#include "QuantumFourierTransformComponent.h"

FVector UQuantumFourierTransformComponent::ApplyQFT()
{
	UE_LOG(LogTemp, Log, TEXT("Applying Quantum Fourier Transform to state: %s"), *QuantumState.ToString());

	const float NewX = FMath::Cos(QuantumState.X) - FMath::Sin(QuantumState.Y);
	const float NewY = FMath::Sin(QuantumState.X) + FMath::Cos(QuantumState.Y);

	QuantumState = FVector(NewX, NewY, QuantumState.Z);

	UE_LOG(LogTemp, Log, TEXT("Transformed State: %s"), *QuantumState.ToString());
	return QuantumState;
}
