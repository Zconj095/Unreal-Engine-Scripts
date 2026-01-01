// Copyright Epic Games, Inc.

#include "QuantumGateObject.h"

UQuantumGateObject* UQuantumGateObject::Hadamard(int32 Qubit)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("Hadamard");
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::PauliZ(int32 Qubit)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("PauliZ");
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::Phase(int32 Qubit, double Angle)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = FString::Printf(TEXT("Phase(%f)"), Angle);
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::CNOT(int32 Control, int32 Target)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("CNOT");
	Gate->NumQubits = 2;
	Gate->Qubits = { Control, Target };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::PauliX(int32 Qubit)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("PauliX");
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::PauliY(int32 Qubit)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("PauliY");
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::RX(int32 Qubit, double Angle)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = FString::Printf(TEXT("RX(%f)"), Angle);
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::RY(int32 Qubit, double Angle)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = FString::Printf(TEXT("RY(%f)"), Angle);
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::RZ(int32 Qubit, double Angle)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = FString::Printf(TEXT("RZ(%f)"), Angle);
	Gate->NumQubits = 1;
	Gate->Qubits = { Qubit };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::SWAP(int32 Qubit1, int32 Qubit2)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("SWAP");
	Gate->NumQubits = 2;
	Gate->Qubits = { Qubit1, Qubit2 };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::Toffoli(int32 Control1, int32 Control2, int32 Target)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("Toffoli");
	Gate->NumQubits = 3;
	Gate->Qubits = { Control1, Control2, Target };
	return Gate;
}

UQuantumGateObject* UQuantumGateObject::Fredkin(int32 Control, int32 Swap1, int32 Swap2)
{
	UQuantumGateObject* Gate = NewObject<UQuantumGateObject>();
	Gate->Name = TEXT("Fredkin");
	Gate->NumQubits = 3;
	Gate->Qubits = { Control, Swap1, Swap2 };
	return Gate;
}
