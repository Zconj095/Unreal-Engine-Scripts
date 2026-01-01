// Copyright Epic Games, Inc.

#include "QuantumSimulatorComponent.h"

#include "LayeredNoiseModel.h"
#include "QuantumCircuitVisualizerComponent.h"
#include "QuantumStateObject.h"

UQuantumSimulatorComponent::UQuantumSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumSimulatorComponent::ConfigureSimulator(UQuantumCircuitObject* InCircuit, UQuantumCircuitVisualizerComponent* InVisualizer)
{
	Circuit = InCircuit;
	Visualizer = InVisualizer;

	if (Circuit)
	{
		if (!State)
		{
			State = NewObject<UQuantumStateObject>(this);
		}
		State->Initialize(Circuit->NumQubits);
	}

	if (!NoiseStack)
	{
		NoiseStack = NewObject<ULayeredNoiseModel>(this);
	}
}

static FString GateTypeToString(EQuantumGateType GateType)
{
	switch (GateType)
	{
	case EQuantumGateType::Hadamard:
		return TEXT("Hadamard");
	case EQuantumGateType::CNOT:
		return TEXT("CNOT");
	default:
		return TEXT("Gate");
	}
}

void UQuantumSimulatorComponent::Simulate()
{
	if (!Circuit)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumSimulatorComponent: Circuit is null; cannot simulate."));
		return;
	}

	if (!State)
	{
		State = NewObject<UQuantumStateObject>(this);
		State->Initialize(Circuit->NumQubits);
	}

	if (!NoiseStack)
	{
		NoiseStack = NewObject<ULayeredNoiseModel>(this);
	}

	if (Visualizer)
	{
		Visualizer->NumQubits = Circuit->NumQubits;
		Visualizer->InitializeCircuitVisualization();
	}

	UE_LOG(LogTemp, Log, TEXT("Simulating circuit with %d gates."), Circuit->Gates.Num());

	for (const FQuantumGate& Gate : Circuit->Gates)
	{
		UE_LOG(LogTemp, Log, TEXT("Applying gate type %d (primary %d, secondary %d)."),
			static_cast<int32>(Gate.GateType), Gate.PrimaryIndex, Gate.SecondaryIndex);

		State->ApplyGate(Gate);

		if (NoiseStack && State)
		{
			NoiseStack->ApplyNoise(State->StateVector);
		}

		if (Visualizer)
		{
			if (Gate.GateType == EQuantumGateType::CNOT)
			{
				Visualizer->AddGate(GateTypeToString(Gate.GateType), Gate.SecondaryIndex, Gate.PrimaryIndex);
			}
			else
			{
				Visualizer->AddGate(GateTypeToString(Gate.GateType), Gate.PrimaryIndex, -1);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Simulation complete."));
}

void UQuantumSimulatorComponent::AddNoiseModel(TScriptInterface<IQuantumNoiseModel> NoiseModel)
{
	if (!NoiseStack)
	{
		NoiseStack = NewObject<ULayeredNoiseModel>(this);
	}

	if (NoiseModel)
	{
		NoiseStack->AddNoiseLayer(NoiseModel);
	}
}
