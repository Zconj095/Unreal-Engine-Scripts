// Copyright Epic Games, Inc.

#include "QuantumFourierTransformCircuitComponent.h"

#include "Logging/LogMacros.h"

UQuantumFourierTransformCircuitComponent::UQuantumFourierTransformCircuitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumFourierTransformCircuitComponent::ApplyQFT(int32 NumQubits)
{
	UE_LOG(LogTemp, Log, TEXT("Applying Quantum Fourier Transform to %d qubits..."), NumQubits);

	for (int32 i = 0; i < NumQubits; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("Applying H Gate to Qubit %d..."), i);
		for (int32 j = i + 1; j < NumQubits; ++j)
		{
			const float Angle = PI / FMath::Pow(2.f, static_cast<float>(j - i));
			UE_LOG(LogTemp, Log, TEXT("Applying Controlled R(%f) Gate between Qubit %d and Qubit %d..."), Angle, i, j);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Quantum Fourier Transform completed."));
}
