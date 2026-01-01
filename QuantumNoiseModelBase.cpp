// Copyright Epic Games, Inc.

#include "QuantumNoiseModelBase.h"

#include "Math/UnrealMathUtility.h"
#include "Logging/LogMacros.h"

void UQuantumNoiseModelBase::ApplyNoise_Implementation(TArray<FComplexNumber>& StateVector)
{
	const double Roll = FMath::FRand();
	if (Roll < Probability)
	{
		UE_LOG(LogTemp, Log, TEXT("Applying %s noise to the quantum state."), *Name);
		// Default implementation is a no-op on the state vector; override in subclasses to mutate StateVector.
	}
}
