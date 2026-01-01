// Copyright Epic Games, Inc.

#include "AmplitudeDampingNoiseModel.h"

#include "ComplexNumber.h"
#include "Math/UnrealMathUtility.h"
#include "Logging/LogMacros.h"

void UAmplitudeDampingNoiseModel::ApplyNoise_Implementation(TArray<FComplexNumber>& StateVector)
{
	const double Roll = FMath::FRand();
	if (Roll >= Probability)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Applying Amplitude Damping Noise..."));

	for (FComplexNumber& Amp : StateVector)
	{
		Amp.Real *= DampingFactor;
		// Imaginary part unchanged, mirroring the Unity implementation.
	}
}
