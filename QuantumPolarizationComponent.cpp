// Copyright Epic Games, Inc.

#include "QuantumPolarizationComponent.h"

#include "Logging/LogMacros.h"

UQuantumPolarizationComponent::UQuantumPolarizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumPolarizationComponent::InitializePolarization()
{
	PolarizationVectors.SetNum(NumQubits);
	for (int32 i = 0; i < NumQubits; ++i)
	{
		PolarizationVectors[i] = FVector(0.f, 0.f, 1.f);
	}
	UE_LOG(LogTemp, Log, TEXT("Initialized polarization vectors."));
}

void UQuantumPolarizationComponent::SetPolarization(int32 Qubit, FVector NewVector)
{
	if (Qubit < 0 || Qubit >= NumQubits)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid qubit index."));
		return;
	}

	if (PolarizationVectors.Num() != NumQubits)
	{
		InitializePolarization();
	}

	PolarizationVectors[Qubit] = NewVector.GetSafeNormal();
	UE_LOG(LogTemp, Log, TEXT("Set polarization for qubit %d: (%f, %f, %f)"), Qubit, NewVector.X, NewVector.Y, NewVector.Z);
}

FVector UQuantumPolarizationComponent::GetPolarization(int32 Qubit) const
{
	if (Qubit < 0 || Qubit >= NumQubits)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid qubit index."));
		return FVector::ZeroVector;
	}

	if (PolarizationVectors.IsValidIndex(Qubit))
	{
		return PolarizationVectors[Qubit];
	}

	return FVector::ZeroVector;
}
