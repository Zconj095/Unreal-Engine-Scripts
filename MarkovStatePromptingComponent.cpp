#include "MarkovStatePromptingComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"
#include "Misc/DateTime.h"

UMarkovStatePromptingComponent::UMarkovStatePromptingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMarkovStatePromptingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMarkovStatePromptingComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability, int32 State)
{
	FScopeLock Lock(&CacheCriticalSection);
	TArray<FMarkovStatePromptEntry>& Entries = QuantumCache.FindOrAdd(Category);
	Entries.Emplace(Prompt, Probability, State);
}

TFuture<FString> UMarkovStatePromptingComponent::GetDefaultPromptAsync(const FString& Category)
{
	TArray<FMarkovStatePromptEntry> EntriesCopy;
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FMarkovStatePromptEntry>* Found = QuantumCache.Find(Category))
		{
			EntriesCopy = *Found;
		}
	}

	return Async(EAsyncExecution::ThreadPool, [Entries = MoveTemp(EntriesCopy)]() mutable -> FString
	{
		if (Entries.IsEmpty())
		{
			return TEXT("No default prompt found");
		}

		float TotalProbability = 0.f;
		for (const FMarkovStatePromptEntry& Entry : Entries)
		{
			TotalProbability += Entry.Probability;
		}

		if (TotalProbability <= 0.f)
		{
			return Entries.Last().Prompt;
		}

		FRandomStream LocalRandom(FDateTime::Now().GetTicks());
		const float RandomValue = LocalRandom.FRandRange(0.f, TotalProbability);
		float Cumulative = 0.f;

		for (const FMarkovStatePromptEntry& Entry : Entries)
		{
			Cumulative += Entry.Probability;
			if (RandomValue <= Cumulative)
			{
				return Entry.Prompt;
			}
		}

		return Entries.Last().Prompt;
	});
}
