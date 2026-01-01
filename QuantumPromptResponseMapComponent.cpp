#include "QuantumPromptResponseMapComponent.h"
#include "Async/Async.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumPromptResponseMapComponent::UQuantumPromptResponseMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumPromptResponseMapComponent::MapPromptResponse(const FString& Category, const FString& Response)
{
	const int32 QuantumKey = GenerateQuantumKey();
	const float Coherence = CalculateQuantumCoherence(QuantumKey);
	const FString Signature = GenerateQuantumSignature(QuantumKey);

	FScopeLock Lock(&MapCriticalSection);
	TArray<FQuantumPromptResponseEntry>& Entries = QuantumResponseMap.FindOrAdd(Category);
	FQuantumPromptResponseEntry Entry;
	Entry.Response = Response;
	Entry.QuantumKey = QuantumKey;
	Entry.Coherence = Coherence;
	Entry.Signature = Signature;
	Entries.Add(MoveTemp(Entry));
}

TFuture<FString> UQuantumPromptResponseMapComponent::RetrieveResponseAsync(const FString& Category)
{
	TArray<FQuantumPromptResponseEntry> EntriesCopy;
	{
		FScopeLock Lock(&MapCriticalSection);
		if (const TArray<FQuantumPromptResponseEntry>* Found = QuantumResponseMap.Find(Category))
		{
			EntriesCopy = *Found;
		}
	}

	if (EntriesCopy.IsEmpty())
	{
		return Async(EAsyncExecution::ThreadPool, []() -> FString
		{
			return TEXT("No mapped response found");
		});
	}

	return Async(EAsyncExecution::ThreadPool, [this, Entries = MoveTemp(EntriesCopy), Category]() mutable -> FString
	{
		TArray<FQuantumPromptResponseEntry> Filtered;
		for (const FQuantumPromptResponseEntry& Entry : Entries)
		{
			if (Entry.Coherence > CoherenceThreshold)
			{
				Filtered.Add(Entry);
			}
		}

		if (Filtered.IsEmpty())
		{
			return TEXT("No valid response mapping available");
		}

		float TotalCoherence = 0.f;
		for (const FQuantumPromptResponseEntry& Entry : Filtered)
		{
			TotalCoherence += Entry.Coherence;
		}

		const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
		FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
		const float RandomValue = Random.FRandRange(0.f, TotalCoherence);
		float Cumulative = 0.f;

		for (const FQuantumPromptResponseEntry& Entry : Filtered)
		{
			Cumulative += Entry.Coherence;
			if (RandomValue <= Cumulative)
			{
				ReinforceQuantumSignature(Category, Entry.Response);
				return Entry.Response;
			}
		}

		return Filtered.Last().Response;
	});
}

int32 UQuantumPromptResponseMapComponent::GenerateQuantumKey() const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(QuantumSignatureFactor * 100.f));
}

float UQuantumPromptResponseMapComponent::CalculateQuantumCoherence(int32 QuantumKey) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumKey);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.FRandRange(0.f, QuantumSignatureFactor) / (QuantumKey + 1.f);
}

FString UQuantumPromptResponseMapComponent::GenerateQuantumSignature(int32 QuantumKey) const
{
	const uint32 RandomSuffix = FMath::RandRange(1000, 9999);
	return FString::Printf(TEXT("QS-%d-%u"), QuantumKey, RandomSuffix);
}

void UQuantumPromptResponseMapComponent::ReinforceQuantumSignature(const FString& Category, const FString& Response)
{
	FScopeLock Lock(&MapCriticalSection);
	if (TArray<FQuantumPromptResponseEntry>* Entries = QuantumResponseMap.Find(Category))
	{
		for (FQuantumPromptResponseEntry& Entry : *Entries)
		{
			if (Entry.Response == Response)
			{
				Entry.Coherence = CalculateQuantumCoherence(Entry.QuantumKey);
				break;
			}
		}
	}
}
