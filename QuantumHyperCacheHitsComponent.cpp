#include "QuantumHyperCacheHitsComponent.h"
#include "Async/Async.h"
#include "Async/ParallelFor.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumHyperCacheHitsComponent::UQuantumHyperCacheHitsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHyperCacheHitsComponent::StorePrompt(const FString& Category, const FString& Prompt, float Probability)
{
	const int32 QuantumState = CalculateQuantumState(Probability);
	const float PhaseCoherence = CalculatePhaseCoherence(QuantumState);

	FScopeLock Lock(&CacheCriticalSection);
	TArray<FQuantumHyperCacheHitEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumHyperCacheHitEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.QuantumState = QuantumState;
	Entry.PhaseCoherence = PhaseCoherence;
	Entries.Add(MoveTemp(Entry));

	PrecomputeCacheAllocation(Category, Prompt);
}

TFuture<FString> UQuantumHyperCacheHitsComponent::RetrievePromptAsync(const FString& Category)
{
	TArray<FQuantumHyperCacheHitEntry> EntriesCopy;
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FQuantumHyperCacheHitEntry>* Found = QuantumCache.Find(Category))
		{
			EntriesCopy = *Found;
		}
	}

	return Async(EAsyncExecution::ThreadPool, [this, Entries = MoveTemp(EntriesCopy), Category]() mutable -> FString
	{
		TArray<FQuantumHyperCacheHitEntry> Filtered;
		for (const FQuantumHyperCacheHitEntry& Entry : Entries)
		{
			if (Entry.PhaseCoherence > CacheHitThreshold)
			{
				Filtered.Add(Entry);
			}
		}

		if (Filtered.IsEmpty())
		{
			return TEXT("No valid cache hits");
		}

		float TotalProbability = 0.f;
		for (const FQuantumHyperCacheHitEntry& Entry : Filtered)
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

		for (const FQuantumHyperCacheHitEntry& Entry : Filtered)
		{
			Cumulative += Entry.Probability;
			if (RandomValue <= Cumulative)
			{
				TriggerParallelQuantumPhaseTransition(Category, Entry.Prompt);
				return Entry.Prompt;
			}
		}

		return Filtered.Last().Prompt;
	});
}

void UQuantumHyperCacheHitsComponent::PrecomputeCacheAllocation(const FString& Category, const FString& Prompt)
{
	FScopeLock Lock(&CacheCriticalSection);
	TArray<FString>& Allocations = PrecomputedCache.FindOrAdd(Category);
	Allocations.Add(Prompt);
}

void UQuantumHyperCacheHitsComponent::TriggerParallelQuantumPhaseTransition(const FString& Category, const FString& Prompt)
{
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FString>* Allocations = PrecomputedCache.Find(Category))
		{
			if (!Allocations->Contains(Prompt))
			{
				return;
			}
		}
		else
		{
			return;
		}
	}

	ParallelFor(GPUClusterProcessingUnits, [this, Category, Prompt](int32 Index)
	{
		SimulateParallelCacheProcessing(Category, Prompt);
	});
}

void UQuantumHyperCacheHitsComponent::SimulateParallelCacheProcessing(const FString& Category, const FString& Prompt)
{
	HebbianReinforcement(Category, Prompt);

	const float NewProbabilitySeed = FMath::Clamp(static_cast<float>(FMath::FRand()), 0.f, 1.f);
	const int32 NewQuantumState = CalculateQuantumState(NewProbabilitySeed);
	const float NewPhaseCoherence = CalculatePhaseCoherence(NewQuantumState);

	FScopeLock Lock(&CacheCriticalSection);
	if (TArray<FQuantumHyperCacheHitEntry>* Entries = QuantumCache.Find(Category))
	{
		for (FQuantumHyperCacheHitEntry& Entry : *Entries)
		{
			if (Entry.Prompt == Prompt)
			{
				Entry.QuantumState = NewQuantumState;
				Entry.PhaseCoherence = NewPhaseCoherence;
				break;
			}
		}
	}
}

void UQuantumHyperCacheHitsComponent::HebbianReinforcement(const FString& Category, const FString& Prompt)
{
	FScopeLock Lock(&CacheCriticalSection);
	if (TArray<FQuantumHyperCacheHitEntry>* Entries = QuantumCache.Find(Category))
	{
		for (FQuantumHyperCacheHitEntry& Entry : *Entries)
		{
			if (Entry.Prompt == Prompt)
			{
				Entry.Probability += 0.02f;
				break;
			}
		}
	}
}

int32 UQuantumHyperCacheHitsComponent::CalculateQuantumState(float Probability) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64()) ^ static_cast<uint64>(Probability * 1000.f);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(Probability * QuantumPhaseTransitionFactor * 10.f));
}

float UQuantumHyperCacheHitsComponent::CalculatePhaseCoherence(int32 QuantumState) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumState);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	const float RandomPhase = Random.FRandRange(0.f, QuantumPhaseTransitionFactor);
	return RandomPhase / (QuantumState + 1.f);
}
