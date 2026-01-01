#include "QuantumHyperCacheComponent.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumHyperCacheComponent::UQuantumHyperCacheComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHyperCacheComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuantumHyperCacheComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	const int32 QuantumState = CalculateQuantumState(Probability);

	FScopeLock Lock(&CacheCriticalSection);
	TArray<FQuantumHyperCacheEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumHyperCacheEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.QuantumState = QuantumState;
	Entries.Add(MoveTemp(Entry));

	const int32 DynamicLimit = AdjustMaxCacheLimit();

	if (Entries.Num() > DynamicLimit)
	{
		ApplyQuantumStateEviction(Category, DynamicLimit);
	}
}

int32 UQuantumHyperCacheComponent::CalculateQuantumState(float Probability) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64()) ^ static_cast<uint64>(Probability * 1000.f);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(Probability * QuantumScalingFactor * 10.f));
}

int32 UQuantumHyperCacheComponent::AdjustMaxCacheLimit()
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	const float EntropyFactor = Random.FRand() * QuantumScalingFactor;
	const int32 AdjustedLimit = FMath::Max(BaseCacheLimit, FMath::RoundToInt(BaseCacheLimit * (1.f + EntropyFactor)));
	return AdjustedLimit;
}

void UQuantumHyperCacheComponent::ApplyQuantumStateEviction(const FString& Category, int32 MaxItems)
{
	FScopeLock Lock(&CacheCriticalSection);
	if (TArray<FQuantumHyperCacheEntry>* Entries = QuantumCache.Find(Category))
	{
		Entries->Sort([](const FQuantumHyperCacheEntry& A, const FQuantumHyperCacheEntry& B)
		{
			const float ScoreA = A.Probability * A.QuantumState;
			const float ScoreB = B.Probability * B.QuantumState;
			return ScoreA > ScoreB;
		});

		if (Entries->Num() > MaxItems)
		{
			Entries->SetNum(MaxItems);
		}
	}
}
