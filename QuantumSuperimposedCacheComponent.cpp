#include "QuantumSuperimposedCacheComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "PlatformFeatures.h"

UQuantumSuperimposedCacheComponent::UQuantumSuperimposedCacheComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumSuperimposedCacheComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	FScopeLock Lock(&CacheCriticalSection);
	TArray<FQuantumSuperimposedEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumSuperimposedEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entries.Add(MoveTemp(Entry));
}

TFuture<FString> UQuantumSuperimposedCacheComponent::GetPromptAsync(const FString& Category)
{
	TArray<FQuantumSuperimposedEntry> Snapshot;
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FQuantumSuperimposedEntry>* Found = QuantumCache.Find(Category))
		{
			Snapshot = *Found;
		}
	}

	if (Snapshot.IsEmpty())
	{
		return Async(EAsyncExecution::ThreadPool, []() -> FString
		{
			return TEXT("No prompt found");
		});
	}

	return Async(EAsyncExecution::ThreadPool, [Snapshot = MoveTemp(Snapshot)]() mutable -> FString
	{
		const float RandomValue = FMath::Clamp(FMath::FRand(), 0.f, 1.f);
		float Cumulative = 0.f;

		for (const FQuantumSuperimposedEntry& Entry : Snapshot)
		{
			Cumulative += Entry.Probability;
			if (RandomValue <= Cumulative)
			{
				return Entry.Prompt;
			}
		}

		return Snapshot.Last().Prompt;
	});
}
