// Copyright Epic Games, Inc.

#include "ShorsAlgorithmComponent.h"

#include "QuantumCircuitObject.h"
#include "QuantumSimulatorComponent.h"

UShorsAlgorithmComponent::UShorsAlgorithmComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShorsAlgorithmComponent::Execute()
{
	ExecuteWith(QuantumCircuit, QuantumSimulator);
}

void UShorsAlgorithmComponent::ExecuteWith(UQuantumCircuitObject* InCircuit, UQuantumSimulatorComponent* InSimulator)
{
	if (!InCircuit || !InSimulator)
	{
		UE_LOG(LogTemp, Error, TEXT("ShorsAlgorithm: Circuit or Simulator is not assigned."));
		return;
	}

	if (NumberToFactor <= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShorsAlgorithm: NumberToFactor must be greater than 2."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Executing Shor's Algorithm to factor %d..."), NumberToFactor);

	for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
	{
		const int32 Guess = FMath::RandRange(2, NumberToFactor - 1);
		int32 GCD = GreatestCommonDivisor(Guess, NumberToFactor);

		if (GCD > 1)
		{
			UE_LOG(LogTemp, Log, TEXT("Found factor: %d"), GCD);
			return;
		}

		// Quantum steps (stub)
		const int32 Period = FindPeriod(Guess);
		if (Period > 0 && (Period % 2) == 0)
		{
			const double HalfPow = FMath::Pow(static_cast<double>(Guess), static_cast<double>(Period) / 2.0);
			const int32 HalfPowInt = static_cast<int32>(HalfPow);
			const int32 Factor1 = HalfPowInt - 1;
			const int32 Factor2 = HalfPowInt + 1;

			GCD = GreatestCommonDivisor(Factor1, NumberToFactor);
			if (GCD > 1)
			{
				UE_LOG(LogTemp, Log, TEXT("Found factors: %d and %d"), GCD, NumberToFactor / GCD);
				return;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Failed to find factors."));
}

int32 UShorsAlgorithmComponent::FindPeriod(int32 Guess) const
{
	// Placeholder: simulate period finding
	const int32 Period = FMath::RandRange(2, 9);
	UE_LOG(LogTemp, Log, TEXT("Simulating period finding for guess %d... returning %d"), Guess, Period);
	return Period;
}

int32 UShorsAlgorithmComponent::GreatestCommonDivisor(int32 A, int32 B) const
{
	while (B != 0)
	{
		const int32 Temp = B;
		B = A % B;
		A = Temp;
	}
	return A;
}
