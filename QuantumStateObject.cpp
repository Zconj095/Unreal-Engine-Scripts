// Copyright Epic Games, Inc.

#include "QuantumStateObject.h"

#include "Logging/LogMacros.h"

void UQuantumStateObject::Initialize(int32 InNumQubits)
{
	NumQubits = FMath::Max(0, InNumQubits);
	const int32 StateSize = (NumQubits > 0) ? (1 << NumQubits) : 1;

	StateVector.SetNum(StateSize);
	for (FComplexNumber& Amp : StateVector)
	{
		Amp.Real = 0.0;
		Amp.Imaginary = 0.0;
	}

	if (StateVector.Num() > 0)
	{
		StateVector[0].Real = 1.0; // |0...0>
	}
}

void UQuantumStateObject::ApplyGate(const FQuantumGate& Gate)
{
	// No gate operation is defined in this data-only model; log a warning.
	UE_LOG(LogTemp, Warning, TEXT("Operation for %d gate is not defined; state unchanged."), static_cast<int32>(Gate.GateType));
}

FString UQuantumStateObject::ToString() const
{
	FString Result = FString::Printf(TEXT("Quantum State (%d qubits):\n"), NumQubits);
	if (StateVector.Num() == 0)
	{
		return Result;
	}

	const int32 NumBits = (StateVector.Num() > 0) ? FMath::Max(1, FMath::CeilToInt(FMath::Log2(static_cast<float>(StateVector.Num())))) : 0;

	for (int32 i = 0; i < StateVector.Num(); ++i)
	{
		FString Binary = FString::FromInt(i);
		if (Binary.Len() < NumBits)
		{
			Binary = FString::ChrN(NumBits - Binary.Len(), TEXT('0')) + Binary;
		}

		Result += FString::Printf(TEXT("|%s>: %f + %fi\n"),
			*Binary,
			StateVector[i].Real,
			StateVector[i].Imaginary);
	}
	return Result;
}
