#include "QuantumHyperCacheStorageComponent.h"
#include "Async/Async.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumHyperCacheStorageComponent::UQuantumHyperCacheStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHyperCacheStorageComponent::StorePrompt(const FString& Category, const FString& Prompt, float Probability)
{
	const int32 QuantumState = CalculateQuantumState(Probability);
	const float PhaseCoherence = CalculatePhaseCoherence(QuantumState);

	FScopeLock Lock(&CacheCriticalSection);
	TArray<FQuantumHyperCacheStorageEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumHyperCacheStorageEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.QuantumState = QuantumState;
	Entry.PhaseCoherence = PhaseCoherence;
	Entries.Add(MoveTemp(Entry));
}

TFuture<FString> UQuantumHyperCacheStorageComponent::RetrievePromptAsync(const FString& Category)
{
	TArray<FQuantumHyperCacheStorageEntry> EntriesCopy;
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FQuantumHyperCacheStorageEntry>* Found = QuantumCache.Find(Category))
		{
			EntriesCopy = *Found;
		}
	}

	if (EntriesCopy.IsEmpty())
	{
		return Async(EAsyncExecution::ThreadPool, []() -> FString
		{
			return TEXT("No data stored");
		});
	}

	return Async(EAsyncExecution::ThreadPool, [this, Entries = MoveTemp(EntriesCopy), Category]() mutable -> FString
	{
		TArray<FQuantumHyperCacheStorageEntry> Filtered;
		for (const FQuantumHyperCacheStorageEntry& Entry : Entries)
		{
			if (Entry.PhaseCoherence > CoherenceThreshold)
			{
				Filtered.Add(Entry);
			}
		}

		if (Filtered.IsEmpty())
		{
			return TEXT("No valid cache data");
		}

		float TotalProbability = 0.f;
		for (const FQuantumHyperCacheStorageEntry& Entry : Filtered)
		{
			TotalProbability += Entry.Probability;
		}

		if (TotalProbability <= 0.f)
		{
			return Filtered.Last().Prompt;
		}

		const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
		FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
		const float RandomValue = Random.FRandRange(0.f, TotalProbability);
		float Cumulative = 0.f;

		for (const FQuantumHyperCacheStorageEntry& Entry : Filtered)
		{
			Cumulative += Entry.Probability;
			if (RandomValue <= Cumulative)
			{
				UpdateQuantumMemory(Category, Entry.Prompt);
				return Entry.Prompt;
			}
		}

		return Filtered.Last().Prompt;
	});
}

int32 UQuantumHyperCacheStorageComponent::CalculateQuantumState(float Probability) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64()) ^ static_cast<uint64>(Probability * 1000.f);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(Probability * PhaseTransitionFactor * 10.f));
}

float UQuantumHyperCacheStorageComponent::CalculatePhaseCoherence(int32 QuantumState) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumState);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	const float RandomPhase = Random.FRandRange(0.f, PhaseTransitionFactor);
	return RandomPhase / (QuantumState + 1.f);
}

void UQuantumHyperCacheStorageComponent::UpdateQuantumMemory(const FString& Category, const FString& Prompt)
{
	FScopeLock Lock(&CacheCriticalSection);
	if (TArray<FQuantumHyperCacheStorageEntry>* Entries = QuantumCache.Find(Category))
	{
		for (FQuantumHyperCacheStorageEntry& Entry : *Entries)
		{
			if (Entry.Prompt == Prompt)
			{
				Entry.PhaseCoherence = CalculatePhaseCoherence(Entry.QuantumState);
				break;
			}
		}
	}
}
