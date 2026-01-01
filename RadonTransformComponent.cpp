// Copyright Epic Games, Inc.

#include "RadonTransformComponent.h"

FVector URadonTransformComponent::ComputeRadonTransform()
{
	UE_LOG(LogTemp, Log, TEXT("Computing Radon Transform with Left: %s, Right: %s"), *LeftState.ToString(), *RightState.ToString());

	const FVector RadonState = (LeftState + RightState) * 0.5f;

	UE_LOG(LogTemp, Log, TEXT("Radon Transform Result: %s"), *RadonState.ToString());
	return RadonState;
}
