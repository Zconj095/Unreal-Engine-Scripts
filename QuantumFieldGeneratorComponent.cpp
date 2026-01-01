// Copyright Epic Games, Inc.

#include "QuantumFieldGeneratorComponent.h"

UQuantumFieldGeneratorComponent::UQuantumFieldGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UQuantumFieldGeneratorComponent::GenerateField() const
{
	const FVector Field(
		FMath::Sin(InputVector.X),
		FMath::Cos(InputVector.Y),
		FMath::Tan(InputVector.Z));

	UE_LOG(LogTemp, Log, TEXT("Generating Quantum Field from Input Vector: %s"), *InputVector.ToString());
	UE_LOG(LogTemp, Log, TEXT("Generated Quantum Field: %s"), *Field.ToString());
	return Field;
}
