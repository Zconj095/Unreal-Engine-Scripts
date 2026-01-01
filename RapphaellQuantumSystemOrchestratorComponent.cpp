// Copyright Epic Games, Inc.

#include "RapphaellQuantumSystemOrchestratorComponent.h"

#include "GroversAlgorithmComponent.h"
#include "HadamardGateComponent.h"
#include "QuantumCircuitObject.h"
#include "QuantumFourierTransformComponent.h"
#include "QuantumPhaseEstimationComponent.h"
#include "RadonTransformComponent.h"
#include "ShorsAlgorithmComponent.h"
#include "MonaQuantumPhaseShiftComponent.h"
#include "QuantumSimulatorComponent.h"

void URapphaellQuantumSystemOrchestratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Attempt to auto-fill from owner if not set
	if (AActor* Owner = GetOwner())
	{
		if (!QFT) QFT = Owner->FindComponentByClass<UQuantumFourierTransformComponent>();
		if (!PhaseEstimation) PhaseEstimation = Owner->FindComponentByClass<UQuantumPhaseEstimationComponent>();
		if (!GroversAlgorithm) GroversAlgorithm = Owner->FindComponentByClass<UGroversAlgorithmComponent>();
		if (!PhaseShift) PhaseShift = Owner->FindComponentByClass<UMonaQuantumPhaseShiftComponent>();
		if (!ShorsAlgorithm) ShorsAlgorithm = Owner->FindComponentByClass<UShorsAlgorithmComponent>();
		if (!RadonTransform) RadonTransform = Owner->FindComponentByClass<URadonTransformComponent>();
		if (!HadamardGate) HadamardGate = Owner->FindComponentByClass<UHadamardGateComponent>();
		if (!Simulator) Simulator = Owner->FindComponentByClass<UQuantumSimulatorComponent>();
	}

	UE_LOG(LogTemp, Log, TEXT("Starting Quantum System: RapphaellQuantumSystemOrchestrator..."));

	if (!QFT || !PhaseEstimation || !GroversAlgorithm || !PhaseShift || !ShorsAlgorithm || !RadonTransform || !HadamardGate)
	{
		UE_LOG(LogTemp, Error, TEXT("RapphaellQuantumSystemOrchestrator: One or more required components are not assigned."));
		return;
	}

	if (!Circuit || !Simulator)
	{
		UE_LOG(LogTemp, Error, TEXT("RapphaellQuantumSystemOrchestrator: Circuit or Simulator is not assigned."));
		return;
	}

	// Step 1: Apply Quantum Fourier Transform
	UE_LOG(LogTemp, Log, TEXT("Applying Quantum Fourier Transform..."));
	const FVector QFTState = QFT->ApplyQFT();
	UE_LOG(LogTemp, Log, TEXT("QFT State: %s"), *QFTState.ToString());

	// Step 2: Process Left Path
	UE_LOG(LogTemp, Log, TEXT("Estimating Phase..."));
	const FVector PhaseEstimatedState = PhaseEstimation->EstimatePhase();
	UE_LOG(LogTemp, Log, TEXT("Phase Estimated State: %s"), *PhaseEstimatedState.ToString());

	UE_LOG(LogTemp, Log, TEXT("Running Grover's Algorithm..."));
	const FVector GroverState = GroversAlgorithm->RunGrover();
	UE_LOG(LogTemp, Log, TEXT("Grover State: %s"), *GroverState.ToString());

	RadonTransform->LeftState = (PhaseEstimatedState + GroverState) * 0.5f;

	// Step 3: Process Right Path
	UE_LOG(LogTemp, Log, TEXT("Running Shor's Algorithm..."));
	ShorsAlgorithm->ExecuteWith(Circuit, Simulator);

	UE_LOG(LogTemp, Log, TEXT("Applying QFT to Right Path..."));
	const FVector UpdatedQFTState = QFT->ApplyQFT();
	RadonTransform->RightState = (PhaseEstimatedState + UpdatedQFTState) * 0.5f;

	// Step 4: Compute Radon Transform
	UE_LOG(LogTemp, Log, TEXT("Computing Radon Transform..."));
	const FVector RadonState = RadonTransform->ComputeRadonTransform();
	UE_LOG(LogTemp, Log, TEXT("Radon Transform State: %s"), *RadonState.ToString());

	// Step 5: Apply Phase Shift
	UE_LOG(LogTemp, Log, TEXT("Applying Phase Shift..."));
	const FVector PhaseShiftedState = PhaseShift->ApplyPhaseShift(PI / 4.f);
	UE_LOG(LogTemp, Log, TEXT("Phase Shifted State: %s"), *PhaseShiftedState.ToString());

	// Step 6: Combine with Hadamard Gate
	UE_LOG(LogTemp, Log, TEXT("Combining with Hadamard Gate..."));
	HadamardGate->InputState = (RadonState + PhaseShiftedState) * 0.5f;
	FVector PhotonOutput, ElectronOutput;
	HadamardGate->ApplyHadamard(PhotonOutput, ElectronOutput);
	UE_LOG(LogTemp, Log, TEXT("Hadamard Gate Outputs - Photon: %s, Electron: %s"), *PhotonOutput.ToString(), *ElectronOutput.ToString());

	// Step 7: Handle Feedback Loops
	UE_LOG(LogTemp, Log, TEXT("Processing Feedback Loops..."));
	QFT->QuantumState = PhotonOutput;
	QFT->ApplyQFT();

	QFT->QuantumState = ElectronOutput;
	QFT->ApplyQFT();

	UE_LOG(LogTemp, Log, TEXT("RapphaellQuantumSystemOrchestrator execution completed successfully."));
}
