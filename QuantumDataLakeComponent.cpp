#include "QuantumDataLakeComponent.h"

#include "Logging/LogMacros.h"

FQuantumData::FQuantumData(const FString& InName, int32 NumStates)
	: Name(InName)
{
	NumStates = FMath::Max(1, NumStates);
	StateProbabilities.SetNumZeroed(NumStates);
	InitializeQuantumState();
}

void FQuantumData::InitializeQuantumState()
{
	float Total = 0.f;
	for (float& P : StateProbabilities)
	{
		P = FMath::FRand();
		Total += P;
	}

	if (Total <= KINDA_SMALL_NUMBER)
	{
		// Avoid divide by zero; make uniform
		const float Uniform = 1.f / StateProbabilities.Num();
		for (float& P : StateProbabilities)
		{
			P = Uniform;
		}
		return;
	}

	for (float& P : StateProbabilities)
	{
		P /= Total;
	}
}

int32 FQuantumData::Measure() const
{
	const float Rand = FMath::FRand();
	float Cumulative = 0.f;
	for (int32 i = 0; i < StateProbabilities.Num(); ++i)
	{
		Cumulative += StateProbabilities[i];
		if (Rand <= Cumulative)
		{
			return i;
		}
	}
	return StateProbabilities.Num() - 1;
}

UQuantumDataLakeComponent::UQuantumDataLakeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumDataLakeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Example initialization if none provided
	if (QuantumDataLake.Num() == 0)
	{
		AddQuantumData(TEXT("QuantumState1"), 4);
		AddQuantumData(TEXT("QuantumState2"), 3);
	}

	PerformQuantumMeasurements();
}

void UQuantumDataLakeComponent::AddQuantumData(const FString& Name, int32 NumStates)
{
	QuantumDataLake.Add(FQuantumData(Name, NumStates));
	UE_LOG(LogTemp, Log, TEXT("Added Quantum Data: %s with %d states."), *Name, NumStates);
}

void UQuantumDataLakeComponent::PerformQuantumMeasurements()
{
	for (const FQuantumData& Q : QuantumDataLake)
	{
		const int32 Measured = Q.Measure();
		UE_LOG(LogTemp, Log, TEXT("%s collapsed to state: %d"), *Q.Name, Measured);
	}
}
