#include "QuantumFluxGaugeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/OutputDeviceDebug.h"

UQuantumFluxGaugeComponent::UQuantumFluxGaugeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumFluxGaugeComponent::AnalyzeSuperimposedStates()
{
	UE_LOG(LogTemp, Log, TEXT("Quantum Flux Gauge: analyzing superimposed states."));

	FluxData.Add(TEXT("Memory Bandwidth"), FMath::FRandRange(0.1f, 1.0f));
	FluxData.Add(TEXT("Superposition Ratio"), FMath::FRandRange(0.5f, 2.0f));
	FluxData.Add(TEXT("Quantum Entropy"), FMath::FRandRange(0.01f, 0.5f));
}

TArray<FQuantumFluxEntry> UQuantumFluxGaugeComponent::GetFluxData() const
{
	TArray<FQuantumFluxEntry> Result;
	for (const auto& Pair : FluxData)
	{
		FQuantumFluxEntry Entry;
		Entry.Key = Pair.Key;
		Entry.Value = Pair.Value;
		Result.Add(Entry);
	}
	return Result;
}

void UQuantumFluxGaugeComponent::DisplayFluxData() const
{
	for (const auto& Pair : FluxData)
	{
		UE_LOG(LogTemp, Log, TEXT("%s: %f"), *Pair.Key, Pair.Value);
	}
}
