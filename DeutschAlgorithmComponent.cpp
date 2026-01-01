// Copyright Epic Games, Inc.

#include "DeutschAlgorithmComponent.h"

void UDeutschAlgorithmComponent::Execute()
{
	UE_LOG(LogTemp, Log, TEXT("Executing Deutsch Algorithm..."));

	InitializeQubits();

	UE_LOG(LogTemp, Log, TEXT("Applying Oracle for %s function..."), bIsConstantFunction ? TEXT("Constant") : TEXT("Balanced"));
	ApplyOracle();

	const FString Result = Measure();
	UE_LOG(LogTemp, Log, TEXT("Deutsch Algorithm determined the function is: %s"), *Result);
}

void UDeutschAlgorithmComponent::InitializeQubits()
{
	UE_LOG(LogTemp, Log, TEXT("Qubits are now in superposition."));
}

void UDeutschAlgorithmComponent::ApplyOracle()
{
	if (bIsConstantFunction)
	{
		UE_LOG(LogTemp, Log, TEXT("Oracle applied: The function is constant."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Oracle applied: The function is balanced."));
	}
}

FString UDeutschAlgorithmComponent::Measure() const
{
	return bIsConstantFunction ? TEXT("Constant") : TEXT("Balanced");
}
