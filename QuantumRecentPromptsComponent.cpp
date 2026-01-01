#include "QuantumRecentPromptsComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"
#include "HAL/PlatformTime.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumRecentPromptsComponent::UQuantumRecentPromptsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumRecentPromptsComponent::StoreRecentPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	const int32 QuantumState = CalculateQuantumState(Probability);
	const float Coherence = CalculateQuantumCoherence(QuantumState);

	FScopeLock Lock(&RecentPromptsCriticalSection);
	TArray<FQuantumRecentPromptEntry>& Entries = QuantumRecentPrompts.FindOrAdd(Category);
	FQuantumRecentPromptEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.QuantumState = QuantumState;
	Entry.Coherence = Coherence;
	Entries.Add(MoveTemp(Entry));

	if (Entries.Num() > RecentPromptLimit)
	{
		Entries.RemoveAt(0);
	}
}

TFuture<FString> UQuantumRecentPromptsComponent::RetrieveRecentPromptAsync(const FString& Category)
{
	TArray<FQuantumRecentPromptEntry> Snapshot;
	{
		FScopeLock Lock(&RecentPromptsCriticalSection);
		if (const TArray<FQuantumRecentPromptEntry>* Found = QuantumRecentPrompts.Find(Category))
		{
			Snapshot = *Found;
		}
	}

	if (Snapshot.IsEmpty())
	{
		return Async(EAsyncExecution::ThreadPool, []() -> FString
		{
			return TEXT("No recent prompts stored");
		});
	}

	return Async(EAsyncExecution::ThreadPool, [this, Category, Snapshot = MoveTemp(Snapshot), Threshold = CoherenceThreshold]() mutable -> FString
	{
		TArray<FQuantumRecentPromptEntry> Filtered;
		for (const FQuantumRecentPromptEntry& Entry : Snapshot)
		{
			if (Entry.Coherence > Threshold)
			{
				Filtered.Add(Entry);
			}
		}

		if (Filtered.IsEmpty())
		{
			return TEXT("No valid recent prompts");
		}

		float TotalProbability = 0.f;
		for (const FQuantumRecentPromptEntry& Entry : Filtered)
		{
			TotalProbability += Entry.Probability;
		}

		const float RandomThreshold = FMath::Max(TotalProbability, KINDA_SMALL_NUMBER);
		const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
		FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
		const float RandomValue = Random.FRandRange(0.f, RandomThreshold);
		float Cumulative = 0.f;

		for (const FQuantumRecentPromptEntry& Entry : Filtered)
		{
			Cumulative += Entry.Probability;
			if (RandomValue <= Cumulative)
			{
				UpdateQuantumMemory(Category, Entry.Prompt);
				return Entry.Prompt;
			}
		}

		UpdateQuantumMemory(Category, Filtered.Last().Prompt);
		return Filtered.Last().Prompt;
	});
}

int32 UQuantumRecentPromptsComponent::CalculateQuantumState(float Probability) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(Probability * QuantumRetentionFactor * 10.f));
}

float UQuantumRecentPromptsComponent::CalculateQuantumCoherence(int32 QuantumState) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumState);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.FRandRange(0.f, QuantumRetentionFactor) / (QuantumState + 1.f);
}

void UQuantumRecentPromptsComponent::UpdateQuantumMemory(const FString& Category, const FString& Prompt)
{
	FScopeLock Lock(&RecentPromptsCriticalSection);
	if (TArray<FQuantumRecentPromptEntry>* Entries = QuantumRecentPrompts.Find(Category))
	{
		for (FQuantumRecentPromptEntry& Entry : *Entries)
		{
			if (Entry.Prompt == Prompt)
			{
				Entry.Coherence = CalculateQuantumCoherence(Entry.QuantumState);
				break;
			}
		}
	}
}
