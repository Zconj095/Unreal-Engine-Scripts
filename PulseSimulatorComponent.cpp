// Copyright Epic Games, Inc.

#include "PulseSimulatorComponent.h"

#include "Logging/LogMacros.h"

UPulseSimulatorComponent::UPulseSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPulseSimulatorComponent::ApplyPulse(double Amplitude, double Frequency, double Duration)
{
	UE_LOG(LogTemp, Log, TEXT("Applying Pulse: Amplitude=%f, Frequency=%f, Duration=%f"), Amplitude, Frequency, Duration);
	// Placeholder for pulse-level control logic
}
