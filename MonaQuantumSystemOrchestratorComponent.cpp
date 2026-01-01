// Copyright Epic Games, Inc.

#include "MonaQuantumSystemOrchestratorComponent.h"

#include "MonaQuantumHyperstateComponent.h"
#include "MonaQuantumPhaseEstimationComponent.h"
#include "MonaQuantumPhaseShiftComponent.h"
#include "QuantumFieldGeneratorComponent.h"
#include "QuantumVectorComponent.h"

void UMonaQuantumSystemOrchestratorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		// Fallback to attached components if not set in editor
		if (!QuantumVector)
		{
			QuantumVector = Owner->FindComponentByClass<UQuantumVectorComponent>();
		}
		if (!FieldGenerator1)
		{
			FieldGenerator1 = Owner->FindComponentByClass<UQuantumFieldGeneratorComponent>();
		}
		if (!FieldGenerator2)
		{
			FieldGenerator2 = Owner->FindComponentByClass<UQuantumFieldGeneratorComponent>();
		}
		if (!PhaseShift)
		{
			PhaseShift = Owner->FindComponentByClass<UMonaQuantumPhaseShiftComponent>();
		}
		if (!PhaseEstimation1)
		{
			PhaseEstimation1 = Owner->FindComponentByClass<UMonaQuantumPhaseEstimationComponent>();
		}
		if (!PhaseEstimation2)
		{
			PhaseEstimation2 = Owner->FindComponentByClass<UMonaQuantumPhaseEstimationComponent>();
		}
		if (!Hyperstate)
		{
			Hyperstate = Owner->FindComponentByClass<UMonaQuantumHyperstateComponent>();
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Starting Quantum System..."));

	if (!QuantumVector || !FieldGenerator1 || !FieldGenerator2 || !PhaseShift || !PhaseEstimation1 || !PhaseEstimation2 || !Hyperstate)
	{
		UE_LOG(LogTemp, Error, TEXT("MonaQuantumSystemOrchestrator: One or more required components are missing."));
		return;
	}

	// Step 1: Generate Quantum Vector
	const FVector QuantumState = QuantumVector->Generate();

	// Step 2: Generate Quantum Fields
	FieldGenerator1->InputVector = QuantumState;
	const FVector Field1 = FieldGenerator1->GenerateField();

	FieldGenerator2->InputVector = QuantumState;
	const FVector Field2 = FieldGenerator2->GenerateField();

	// Step 3: Apply Quantum Phase Shift
	PhaseShift->QuantumState = QuantumState;
	const FVector PhaseShiftedState = PhaseShift->ApplyPhaseShift(PI / 4.f);

	// Step 4: Phase Estimation
	PhaseEstimation1->QuantumState = PhaseShiftedState + Field1;
	const FVector Phase1 = PhaseEstimation1->EstimatePhase();

	PhaseEstimation2->QuantumState = PhaseShiftedState + Field2;
	const FVector Phase2 = PhaseEstimation2->EstimatePhase();

	// Step 5: Form Quantum Hyperstate
	Hyperstate->Input1 = Phase1;
	Hyperstate->Input2 = Phase2;
	const FVector FinalHyperstate = Hyperstate->FormHyperstate();

	UE_LOG(LogTemp, Log, TEXT("Final Quantum Hyperstate: %s"), *FinalHyperstate.ToString());
}
