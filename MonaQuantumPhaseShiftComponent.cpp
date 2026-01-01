// Copyright Epic Games, Inc.

#include "MonaQuantumPhaseShiftComponent.h"

FVector UMonaQuantumPhaseShiftComponent::ApplyPhaseShift(float Angle)
{
	UE_LOG(LogTemp, Log, TEXT("Applying Phase Shift of %f radians to state: %s"), Angle, *QuantumState.ToString());

	const float CosA = FMath::Cos(Angle);
	const float SinA = FMath::Sin(Angle);

	const float NewX = QuantumState.X * CosA - QuantumState.Y * SinA;
	const float NewY = QuantumState.X * SinA + QuantumState.Y * CosA;

	QuantumState.X = NewX;
	QuantumState.Y = NewY;

	UE_LOG(LogTemp, Log, TEXT("Phase Shifted State: %s"), *QuantumState.ToString());
	return QuantumState;
}
