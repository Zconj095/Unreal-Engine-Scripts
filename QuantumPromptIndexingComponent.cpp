#include "QuantumPromptIndexingComponent.h"
#include "Async/Async.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumPromptIndexingComponent::UQuantumPromptIndexingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumPromptIndexingComponent::IndexPrompt(const FString& Category, const FString& Prompt)
{
	const int32 QuantumIndex = GenerateQuantumIndex();
	const float Coherence = CalculateCoherence(QuantumIndex);
	const int32 VectorIndex = GenerateVectorIndex(QuantumIndex);

	FScopeLock Lock(&IndexCriticalSection);
	TArray<FQuantumPromptIndexEntry>& Entries = QuantumIndexMap.FindOrAdd(Category);
	FQuantumPromptIndexEntry Entry;
	Entry.Prompt = Prompt;
	Entry.QuantumIndex = QuantumIndex;
	Entry.Coherence = Coherence;
	Entry.VectorIndex = VectorIndex;
	Entries.Add(MoveTemp(Entry));
}

TFuture<FString> UQuantumPromptIndexingComponent::RetrieveIndexedPromptAsync(const FString& Category)
{
	TArray<FQuantumPromptIndexEntry> Snapshot;
	{
		FScopeLock Lock(&IndexCriticalSection);
		if (const TArray<FQuantumPromptIndexEntry>* Found = QuantumIndexMap.Find(Category))
		{
			Snapshot = *Found;
		}
	}

	if (Snapshot.IsEmpty())
	{
		return Async(EAsyncExecution::ThreadPool, []() -> FString
		{
			return TEXT("No indexed prompt found");
		});
	}

	return Async(EAsyncExecution::ThreadPool, [this, Snapshot = MoveTemp(Snapshot), Category]() mutable -> FString
	{
		TArray<FQuantumPromptIndexEntry> Filtered;
		for (const FQuantumPromptIndexEntry& Entry : Snapshot)
		{
			if (Entry.Coherence > CoherenceThreshold)
			{
				Filtered.Add(Entry);
			}
		}

		if (Filtered.IsEmpty())
		{
			return TEXT("No valid index states");
		}

		float TotalCoherence = 0.f;
		for (const FQuantumPromptIndexEntry& Entry : Filtered)
		{
			TotalCoherence += Entry.Coherence;
		}

		const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
		FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
		const float RandomValue = Random.FRandRange(0.f, FMath::Max(TotalCoherence, KINDA_SMALL_NUMBER));
		float Cumulative = 0.f;

		for (const FQuantumPromptIndexEntry& Entry : Filtered)
		{
			Cumulative += Entry.Coherence;
			if (RandomValue <= Cumulative)
			{
				UpdateIndexCoherence(Category, Entry.Prompt);
				return Entry.Prompt;
			}
		}

		UpdateIndexCoherence(Category, Filtered.Last().Prompt);
		return Filtered.Last().Prompt;
	});
}

int32 UQuantumPromptIndexingComponent::GenerateQuantumIndex() const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(QuantumIndexFactor * 100.f));
}

float UQuantumPromptIndexingComponent::CalculateCoherence(int32 QuantumIndex) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumIndex);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.FRandRange(0.f, QuantumIndexFactor) / (QuantumIndex + 1.f);
}

int32 UQuantumPromptIndexingComponent::GenerateVectorIndex(int32 QuantumIndex) const
{
	return BaseVectorIndex + (QuantumIndex % 100);
}

void UQuantumPromptIndexingComponent::UpdateIndexCoherence(const FString& Category, const FString& Prompt)
{
	FScopeLock Lock(&IndexCriticalSection);
	if (TArray<FQuantumPromptIndexEntry>* Entries = QuantumIndexMap.Find(Category))
	{
		for (FQuantumPromptIndexEntry& Entry : *Entries)
		{
			if (Entry.Prompt == Prompt)
			{
				Entry.Coherence = CalculateCoherence(Entry.QuantumIndex);
				break;
			}
		}
	}
}
