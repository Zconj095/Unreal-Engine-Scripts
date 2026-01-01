// Copyright Epic Games, Inc.

#include "QuantumChannelComponent.h"

#include "Logging/LogMacros.h"

UQuantumChannelComponent::UQuantumChannelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UQuantumChannelComponent::ApplyChannel(const FVector& InputState) const
{
	const float Probability = static_cast<float>(DepolarizingProbability);
	const FVector OutputState = InputState * (1.f - Probability);
	UE_LOG(LogTemp, Log, TEXT("Applied depolarizing channel. Output State: (%f, %f, %f)"), OutputState.X, OutputState.Y, OutputState.Z);
	return OutputState;
}
