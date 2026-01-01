// Copyright Epic Games, Inc.

#include "QuantumGatesLibrary.h"

FQubit UQuantumGatesLibrary::ApplyHadamard(const FQubit& Qubit)
{
	const float InvSqrt2 = 1.0f / FMath::Sqrt(2.0f);
	const float NewAmp0 = (Qubit.Amplitude0 + Qubit.Amplitude1) * InvSqrt2;
	const float NewAmp1 = (Qubit.Amplitude0 - Qubit.Amplitude1) * InvSqrt2;
	return FQubit(NewAmp0, NewAmp1);
}

FQubit UQuantumGatesLibrary::ApplyToffoli(const FQubit& Control1, const FQubit& Control2, const FQubit& Target)
{
	if (Control1.Amplitude1 > 0.5f && Control2.Amplitude1 > 0.5f)
	{
		return FQubit(Target.Amplitude1, Target.Amplitude0);
	}
	return Target;
}

FQubit UQuantumGatesLibrary::QuantumTeleport(const FQubit& Qubit)
{
	return FQubit(Qubit.Amplitude0, Qubit.Amplitude1);
}
