#include "QuantumHyperPromptCompressionComponent.h"
#include "Async/Async.h"
#include "Misc/ScopeLock.h"
#include "HAL/PlatformTime.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumHyperPromptCompressionComponent::UQuantumHyperPromptCompressionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHyperPromptCompressionComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	const int32 QuantumState = CalculateQuantumState(Probability);
	const FString CompressedPrompt = CompressPrompt(Prompt, Probability);

	FScopeLock Lock(&CacheCriticalSection);
	TArray<FQuantumHyperPromptCompressionEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumHyperPromptCompressionEntry Entry;
	Entry.CompressedPrompt = CompressedPrompt;
	Entry.OriginalPrompt = Prompt;
	Entry.Probability = Probability;
	Entry.QuantumState = QuantumState;
	Entries.Add(MoveTemp(Entry));
}

TFuture<FString> UQuantumHyperPromptCompressionComponent::GetCompressedPromptAsync(const FString& Category)
{
	TArray<FQuantumHyperPromptCompressionEntry> EntriesCopy;
	{
		FScopeLock Lock(&CacheCriticalSection);
		if (const TArray<FQuantumHyperPromptCompressionEntry>* Found = QuantumCache.Find(Category))
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

	return Async(EAsyncExecution::ThreadPool, [this, Entries = MoveTemp(EntriesCopy)]() mutable -> FString
	{
		float TotalProbability = 0.f;
		for (const FQuantumHyperPromptCompressionEntry& Entry : Entries)
		{
			TotalProbability += Entry.Probability;
		}

		if (TotalProbability <= 0.f)
		{
			const FQuantumHyperPromptCompressionEntry& LastEntry = Entries.Last();
			return DecompressPrompt(LastEntry.CompressedPrompt, LastEntry.OriginalPrompt, LastEntry.Probability);
		}

		const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
		FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
		const float RandomValue = Random.FRandRange(0.f, TotalProbability);
		float Cumulative = 0.f;

		for (const FQuantumHyperPromptCompressionEntry& Entry : Entries)
		{
			Cumulative += Entry.Probability;
			if (RandomValue <= Cumulative)
			{
				return DecompressPrompt(Entry.CompressedPrompt, Entry.OriginalPrompt, Entry.Probability);
			}
		}

		const FQuantumHyperPromptCompressionEntry& LastEntry = Entries.Last();
		return DecompressPrompt(LastEntry.CompressedPrompt, LastEntry.OriginalPrompt, LastEntry.Probability);
	});
}

FString UQuantumHyperPromptCompressionComponent::CompressPrompt(const FString& Prompt, float Probability) const
{
	const int32 CompressionLevel = FMath::Clamp(FMath::RoundToInt(Probability * CompressionThreshold * FMath::FRandRange(3.f, 8.f)), 1, 64);
	TArray<FString> Words;
	Prompt.ParseIntoArray(Words, TEXT(" "), true);

	if (Words.Num() > CompressionLevel)
	{
		FString Result;
		for (int32 Index = 0; Index < CompressionLevel; ++Index)
		{
			Result += Words[Index];
			if (Index < CompressionLevel - 1)
			{
				Result += TEXT(" ");
			}
		}
		Result += TEXT("...");
		return Result;
	}

	return Prompt;
}

FString UQuantumHyperPromptCompressionComponent::DecompressPrompt(const FString& CompressedPrompt, const FString& OriginalPrompt, float Probability) const
{
	const float RecoveryChance = FMath::FRandRange(0.f, QuantumScalingFactor);
	if (RecoveryChance > CompressionThreshold)
	{
		return OriginalPrompt;
	}

	return CompressedPrompt;
}

int32 UQuantumHyperPromptCompressionComponent::CalculateQuantumState(float Probability) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64()) ^ static_cast<uint64>(Probability * 1000.f);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(Probability * QuantumScalingFactor * 10.f));
}
