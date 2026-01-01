// Copyright Epic Games, Inc.

#include "DensityMatrixSimulatorComponent.h"

#include "Logging/LogMacros.h"

UDensityMatrixSimulatorComponent::UDensityMatrixSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDensityMatrixSimulatorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDensityMatrixSimulatorComponent::ConfigureCircuit(int32 InNumQubits, const TArray<FQuantumGate>& InGates)
{
	NumQubits = InNumQubits;
	CircuitGates = InGates;
	InitializeDensityMatrix(NumQubits);
}

void UDensityMatrixSimulatorComponent::InitializeDensityMatrix(int32 InNumQubits)
{
	MatrixDim = FMath::Clamp(1 << FMath::Clamp(InNumQubits, 0, 12), 1, 1 << 12); // clamp to avoid excessive allocation
	const int32 Total = MatrixDim * MatrixDim;
	DensityMatrix.SetNum(Total);

	// Zero-initialize
	for (FComplexNumber& Cell : DensityMatrix)
	{
		Cell.Real = 0.0;
		Cell.Imaginary = 0.0;
	}

	// Start in pure |0...0> state
	if (Total > 0)
	{
		DensityMatrix[0].Real = 1.0;
	}
}

void UDensityMatrixSimulatorComponent::Simulate()
{
	for (const FQuantumGate& Gate : CircuitGates)
	{
		UE_LOG(LogTemp, Log, TEXT("Applying gate with Density Matrix Simulation..."));
		ApplyGateToDensityMatrix(Gate);
	}

	UE_LOG(LogTemp, Log, TEXT("Density Matrix Simulation Complete."));
}

void UDensityMatrixSimulatorComponent::ApplyGateToDensityMatrix(const FQuantumGate& Gate)
{
	// Placeholder for gate application logic to density matrix
	UE_LOG(LogTemp, Log, TEXT("Applying gate to density matrix. GateType: %d, PrimaryIndex: %d, SecondaryIndex: %d"),
		static_cast<int32>(Gate.GateType), Gate.PrimaryIndex, Gate.SecondaryIndex);
}
