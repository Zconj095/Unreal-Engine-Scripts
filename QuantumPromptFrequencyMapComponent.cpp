#include "QuantumPromptFrequencyMapComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"
#include "HAL/PlatformTime.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumPromptFrequencyMapComponent::UQuantumPromptFrequencyMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumPromptFrequencyMapComponent::RegisterPromptUsage(const FString& Category, const FString& Prompt)
{
	const int32 QuantumState = GenerateQuantumState();
	const float Coherence = CalculateQuantumCoherence(QuantumState);
	const int32 HyperdimensionalWeight = CalculateHyperdimensionalInfluence(QuantumState, Coherence);

	FScopeLock Lock(&MapCriticalSection);
	TArray<FQuantumPromptFrequencyEntry>& Entries = QuantumFrequencyMap.FindOrAdd(Category);
	int32 Index = Entries.IndexOfByPredicate([&Prompt](const FQuantumPromptFrequencyEntry& Entry)
	{
		return Entry.Prompt == Prompt;
	});

	if (Index != INDEX_NONE)
	{
		FQuantumPromptFrequencyEntry& Existing = Entries[Index];
		Existing.Frequency += 1;
		Existing.QuantumState = QuantumState;
		Existing.Coherence = Coherence;
		Existing.HyperdimensionalWeight = HyperdimensionalWeight;
	}
	else
	{
		FQuantumPromptFrequencyEntry Entry;
		Entry.Prompt = Prompt;
		Entry.Frequency = 1;
		Entry.QuantumState = QuantumState;
		Entry.Coherence = Coherence;
		Entry.HyperdimensionalWeight = HyperdimensionalWeight;
		Entries.Add(MoveTemp(Entry));
	}
}

TFuture<FString> UQuantumPromptFrequencyMapComponent::RetrieveFrequentPromptAsync(const FString& Category)
{
	TArray<FQuantumPromptFrequencyEntry> EntriesCopy;
	{
		FScopeLock Lock(&MapCriticalSection);
		if (const TArray<FQuantumPromptFrequencyEntry>* Found = QuantumFrequencyMap.Find(Category))
		{
			EntriesCopy = *Found;
		}
	}

	if (EntriesCopy.IsEmpty())
	{
		return Async(EAsyncExecution::ThreadPool, []() -> FString
		{
			return TEXT("No stored prompts available");
		});
	}

	return Async(EAsyncExecution::ThreadPool, [Entries = MoveTemp(EntriesCopy), Threshold = CoherenceThreshold]() mutable -> FString
	{
		TArray<FQuantumPromptFrequencyEntry> Filtered = Entries.FilterByPredicate([Threshold](const FQuantumPromptFrequencyEntry& Entry)
		{
			return Entry.Coherence > Threshold;
		});

		if (Filtered.Num() == 0)
		{
			return TEXT("No frequently accessed prompts available.");
		}

		Filtered.Sort([](const FQuantumPromptFrequencyEntry& A, const FQuantumPromptFrequencyEntry& B)
		{
			const int32 ScoreA = A.Frequency * A.HyperdimensionalWeight;
			const int32 ScoreB = B.Frequency * B.HyperdimensionalWeight;
			return ScoreA > ScoreB;
		});

		return Filtered[0].Prompt;
	});
}

int32 UQuantumPromptFrequencyMapComponent::GenerateQuantumState() const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(HyperdimensionalFactor * 100.f));
}

float UQuantumPromptFrequencyMapComponent::CalculateQuantumCoherence(int32 QuantumState) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumState);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.FRandRange(0.f, HyperdimensionalFactor) / (QuantumState + 1.f);
}

int32 UQuantumPromptFrequencyMapComponent::CalculateHyperdimensionalInfluence(int32 QuantumState, float Coherence) const
{
	return FMath::RoundToInt(QuantumState * Coherence * HyperdimensionalFactor);
}
