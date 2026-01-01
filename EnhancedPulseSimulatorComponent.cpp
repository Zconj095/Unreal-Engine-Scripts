// Copyright Epic Games, Inc.

#include "EnhancedPulseSimulatorComponent.h"

#include "AdvancedPulseVisualizer.h"
#include "Engine/World.h"
#include "Logging/LogMacros.h"

UEnhancedPulseSimulatorComponent::UEnhancedPulseSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnhancedPulseSimulatorComponent::ConfigureSimulator(int32 InNumQubits, AAdvancedPulseVisualizer* InVisualizer)
{
	VisualizerActor = InVisualizer ? InVisualizer : VisualizerActor;

	if (!VisualizerActor && VisualizerClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		VisualizerActor = GetWorld()->SpawnActor<AAdvancedPulseVisualizer>(VisualizerClass, SpawnParams);
	}

	InitializeState(InNumQubits);

	UE_LOG(LogTemp, Log, TEXT("EnhancedPulseSimulator initialized with %d qubits."), NumQubits);
}

void UEnhancedPulseSimulatorComponent::InitializeState(int32 InNumQubits)
{
	NumQubits = FMath::Clamp(InNumQubits, 0, 12); // cap to avoid huge allocations
	const int32 Count = (NumQubits > 0) ? (1 << NumQubits) : 1;
	StateVector.SetNum(Count);

	for (FComplexNumber& Amp : StateVector)
	{
		Amp.Real = 0.0;
		Amp.Imaginary = 0.0;
	}

	if (Count > 0)
	{
		StateVector[0].Real = 1.0; // |0...0>
	}
}

void UEnhancedPulseSimulatorComponent::ApplyPulse(double Amplitude, double Frequency, double Duration)
{
	UE_LOG(LogTemp, Log, TEXT("Applying Pulse: Amplitude=%f, Frequency=%f, Duration=%f"), Amplitude, Frequency, Duration);

	if (VisualizerActor)
	{
		VisualizerActor->VisualizePulse(static_cast<float>(Amplitude), static_cast<float>(Frequency), static_cast<float>(Duration));
	}

	if (StateVector.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateVector is empty; cannot apply rotation/phase."));
		return;
	}

	const double RotationAngle = Amplitude * Duration;
	const double PhaseShift = Frequency * Duration;

	const double CosRotation = FMath::Cos(static_cast<float>(RotationAngle));
	const double SinRotation = FMath::Sin(static_cast<float>(RotationAngle));

	const double PhaseArg = PhaseShift * 2.0 * PI;
	const double CosPhase = FMath::Cos(static_cast<float>(PhaseArg));
	const double SinPhase = FMath::Sin(static_cast<float>(PhaseArg));

	for (FComplexNumber& Amp : StateVector)
	{
		const double Real = Amp.Real;
		const double Imag = Amp.Imaginary;

		// Rotation
		const double RotatedReal = Real * CosRotation - Imag * SinRotation;
		const double RotatedImag = Real * SinRotation + Imag * CosRotation;

		// Phase shift
		const double NewReal = RotatedReal * CosPhase - RotatedImag * SinPhase;
		const double NewImag = RotatedReal * SinPhase + RotatedImag * CosPhase;

		Amp.Real = NewReal;
		Amp.Imaginary = NewImag;
	}
}
