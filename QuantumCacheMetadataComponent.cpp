#include "QuantumCacheMetadataComponent.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumCacheMetadataComponent::UQuantumCacheMetadataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumCacheMetadataComponent::StoreMetadata(const FString& Category, const FString& MetadataKey, const FString& MetadataValue, float Probability)
{
	const int32 QuantumState = GenerateQuantumState(Probability);
	const float Coherence = CalculateQuantumCoherence(QuantumState);

	FScopeLock Lock(&CacheCriticalSection);
	TArray<FQuantumMetadataEntry>& Entries = QuantumMetadataCache.FindOrAdd(Category);
	Entries.Emplace(MetadataKey, MetadataValue, Probability, QuantumState, Coherence);
}

TFuture<FString> UQuantumCacheMetadataComponent::RetrieveMetadataAsync(const FString& Category, const FString& MetadataKey)
{
	const float LocalCoherenceThreshold = CoherenceThreshold;

	TArray<FQuantumMetadataEntry> EntriesCopy;
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FQuantumMetadataEntry>* Found = QuantumMetadataCache.Find(Category))
		{
			EntriesCopy = *Found;
		}
	}

	TWeakObjectPtr<UQuantumCacheMetadataComponent> WeakThis(this);
	return Async(EAsyncExecution::ThreadPool, [Entries = MoveTemp(EntriesCopy), MetadataKey, Category, LocalCoherenceThreshold, WeakThis]() mutable -> FString
	{
		TArray<FQuantumMetadataEntry> Filtered;
		for (const FQuantumMetadataEntry& Entry : Entries)
		{
			if (Entry.MetadataKey == MetadataKey && Entry.Coherence > LocalCoherenceThreshold)
			{
				Filtered.Add(Entry);
			}
		}

		if (Filtered.IsEmpty())
		{
			return TEXT("No valid metadata available");
		}

		float TotalProbability = 0.f;
		for (const FQuantumMetadataEntry& FilteredEntry : Filtered)
		{
			TotalProbability += FilteredEntry.Probability;
		}

		if (TotalProbability <= 0.f)
		{
			return Filtered.Last().MetadataValue;
		}

		const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
		FRandomStream LocalRandom(static_cast<int32>(Seed & 0xffffffff));
		const float RandomValue = LocalRandom.FRandRange(0.f, TotalProbability);
		float Cumulative = 0.f;

		for (const FQuantumMetadataEntry& FilteredEntry : Filtered)
		{
			Cumulative += FilteredEntry.Probability;
			if (RandomValue <= Cumulative)
			{
				if (WeakThis.IsValid())
				{
					AsyncTask(ENamedThreads::GameThread, [WeakThis, Category, MetadataKey]()
					{
						if (UQuantumCacheMetadataComponent* StrongThis = WeakThis.Get())
						{
							StrongThis->ReinforceMetadata(Category, MetadataKey);
						}
					});
				}

				return FilteredEntry.MetadataValue;
			}
		}

		return Filtered.Last().MetadataValue;
	});
}

int32 UQuantumCacheMetadataComponent::GenerateQuantumState(float Probability) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64()) ^ static_cast<uint64>(Probability * 1000.f);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(QuantumMetadataFactor * 100.f));
}

float UQuantumCacheMetadataComponent::CalculateQuantumCoherence(int32 QuantumState) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumState);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	const float RandomCoherence = Random.FRandRange(0.f, QuantumMetadataFactor);
	return RandomCoherence / (QuantumState + 1.f);
}

void UQuantumCacheMetadataComponent::ReinforceMetadata(const FString& Category, const FString& MetadataKey)
{
	FScopeLock Lock(&CacheCriticalSection);

	if (TArray<FQuantumMetadataEntry>* Entries = QuantumMetadataCache.Find(Category))
	{
		for (int32 Index = 0; Index < Entries->Num(); ++Index)
		{
			if ((*Entries)[Index].MetadataKey == MetadataKey)
			{
				const int32 QuantumState = (*Entries)[Index].QuantumState;
				const float NewCoherence = CalculateQuantumCoherence(QuantumState);
				(*Entries)[Index].Coherence = NewCoherence;
				break;
			}
		}
	}
}
