#include "QuantumHyperstateCacheEvictionComponent.h"
#include "Async/Async.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumHyperstateCacheEvictionComponent::UQuantumHyperstateCacheEvictionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHyperstateCacheEvictionComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	const float Hyperstate = EncodeHyperstate(Probability);

	FScopeLock Lock(&CacheCriticalSection);
	TArray<FQuantumHyperstateEvictionEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumHyperstateEvictionEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.Hyperstate = Hyperstate;
	Entries.Add(MoveTemp(Entry));

	if (Entries.Num() > MaxCacheSize)
	{
		HyperstateDrivenEviction(Category);
	}
}

TFuture<FString> UQuantumHyperstateCacheEvictionComponent::GetPromptAsync(const FString& Category, const FString& InputVector)
{
	TArray<FQuantumHyperstateEvictionEntry> EntriesCopy;
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FQuantumHyperstateEvictionEntry>* Found = QuantumCache.Find(Category))
		{
			EntriesCopy = *Found;
		}
	}

	if (EntriesCopy.IsEmpty())
	{
		return Async(EAsyncExecution::ThreadPool, []() -> FString
		{
			return TEXT("No prompt found");
		});
	}

	return Async(EAsyncExecution::ThreadPool, [this, Category, Entries = MoveTemp(EntriesCopy)]() mutable -> FString
	{
		TArray<FQuantumHyperstateEvictionEntry> Filtered;
		for (const FQuantumHyperstateEvictionEntry& Entry : Entries)
		{
			if (Entry.Hyperstate > HyperTangentFactor)
			{
				Filtered.Add(Entry);
			}
		}

		if (Filtered.IsEmpty())
		{
			return TEXT("No valid prompts available");
		}

		float TotalProbability = 0.f;
		for (const FQuantumHyperstateEvictionEntry& Entry : Filtered)
		{
			TotalProbability += Entry.Probability;
		}

		if (TotalProbability <= 0.f)
		{
			const FQuantumHyperstateEvictionEntry& LastEntry = Filtered.Last();
			UpdateHyperstate(Category, LastEntry.Prompt);
			return LastEntry.Prompt;
		}

		const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
		FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
		const float RandomValue = Random.FRandRange(0.f, TotalProbability);
		float Cumulative = 0.f;

		for (const FQuantumHyperstateEvictionEntry& Entry : Filtered)
		{
			Cumulative += Entry.Probability;
			if (RandomValue <= Cumulative)
			{
				UpdateHyperstate(Category, Entry.Prompt);
				return Entry.Prompt;
			}
		}

		const FQuantumHyperstateEvictionEntry& LastEntry = Filtered.Last();
		UpdateHyperstate(Category, LastEntry.Prompt);
		return LastEntry.Prompt;
	});
}

void UQuantumHyperstateCacheEvictionComponent::HyperstateDrivenEviction(const FString& Category)
{
	FScopeLock Lock(&CacheCriticalSection);
	if (TArray<FQuantumHyperstateEvictionEntry>* Entries = QuantumCache.Find(Category))
	{
		Entries->Sort([this](const FQuantumHyperstateEvictionEntry& A, const FQuantumHyperstateEvictionEntry& B)
		{
			const float ScoreA = A.Hyperstate * (1.f + A.UsageCount * HyperTangentFactor);
			const float ScoreB = B.Hyperstate * (1.f + B.UsageCount * HyperTangentFactor);
			return ScoreA > ScoreB;
		});

		if (Entries->Num() > MaxCacheSize)
		{
			Entries->SetNum(MaxCacheSize);
		}
	}
}

float UQuantumHyperstateCacheEvictionComponent::EncodeHyperstate(float Probability) const
{
	return FMath::Tanh(Probability * HyperTangentFactor);
}

void UQuantumHyperstateCacheEvictionComponent::UpdateHyperstate(const FString& Category, const FString& Prompt)
{
	FScopeLock Lock(&CacheCriticalSection);
	if (TArray<FQuantumHyperstateEvictionEntry>* Entries = QuantumCache.Find(Category))
	{
		for (FQuantumHyperstateEvictionEntry& Entry : *Entries)
		{
			if (Entry.Prompt == Prompt)
			{
				const float NewHyperstate = EncodeHyperstate(Entry.Probability + 0.05f);
				Entry.Hyperstate = NewHyperstate;
				Entry.Probability += 0.05f;
				Entry.UsageCount += 1;
				break;
			}
		}
	}
}
