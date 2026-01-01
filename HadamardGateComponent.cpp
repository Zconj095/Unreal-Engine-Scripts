// Copyright Epic Games, Inc.

#include "HadamardGateComponent.h"

void UHadamardGateComponent::ApplyHadamard(FVector& PhotonOutput, FVector& ElectronOutput)
{
	UE_LOG(LogTemp, Log, TEXT("Applying Hadamard Gate to state: %s"), *InputState.ToString());

	const float InvSqrt2 = 1.0f / FMath::Sqrt(2.0f);

	PhotonOutput = FVector(
		(InputState.X + InputState.Y) * InvSqrt2,
		(InputState.X - InputState.Y) * InvSqrt2,
		InputState.Z);

	ElectronOutput = FVector(
		(InputState.X - InputState.Z) * InvSqrt2,
		(InputState.Y + InputState.Z) * InvSqrt2,
		InputState.Z);

	UE_LOG(LogTemp, Log, TEXT("Photon Output: %s, Electron Output: %s"), *PhotonOutput.ToString(), *ElectronOutput.ToString());
}
