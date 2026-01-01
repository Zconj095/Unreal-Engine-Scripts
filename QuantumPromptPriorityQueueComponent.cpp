#include "QuantumPromptPriorityQueueComponent.h"
#include "Async/Async.h"
#include "HAL/PlatformTime.h"
#include "Misc/ScopeLock.h"
#include "Math/UnrealMathUtility.h"

UQuantumPromptPriorityQueueComponent::UQuantumPromptPriorityQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumPromptPriorityQueueComponent::EnqueuePrompt(const FString& Prompt, float InitialPriority)
{
	const int32 QuantumState = GenerateQuantumState();
	const float Coherence = CalculateQuantumCoherence(QuantumState);
	const int32 HyperdimensionalWeight = CalculateHyperdimensionalInfluence(QuantumState, Coherence);
	const float AdjustedPriority = AdjustPromptPriority(InitialPriority, Coherence, HyperdimensionalWeight);

	FScopeLock Lock(&QueueCriticalSection);
	TArray<FQuantumPromptQueueEntry>& Entries = QuantumPromptQueue.FindOrAdd(AdjustedPriority);
	FQuantumPromptQueueEntry Entry;
	Entry.Prompt = Prompt;
	Entry.QuantumState = QuantumState;
	Entry.Coherence = Coherence;
	Entry.HyperdimensionalWeight = HyperdimensionalWeight;
	Entries.Add(MoveTemp(Entry));

	if (QuantumPromptQueue.Num() > MaxQueueSize)
	{
		const float LowestKey = QuantumPromptQueue.begin().Key();
		QuantumPromptQueue.Remove(LowestKey);
	}
}

TFuture<FString> UQuantumPromptPriorityQueueComponent::DequeuePromptAsync()
{
	return Async(EAsyncExecution::ThreadPool, [this]() -> FString
	{
		FScopeLock Lock(&QueueCriticalSection);

		if (QuantumPromptQueue.Num() == 0)
		{
			return TEXT("No prompts available");
		}

		float HighestPriority = TNumericLimits<float>::Lowest();
		for (const auto& Pair : QuantumPromptQueue)
		{
			HighestPriority = FMath::Max(HighestPriority, Pair.Key);
		}

		TArray<FQuantumPromptQueueEntry>* Entries = QuantumPromptQueue.Find(HighestPriority);
		if (!Entries || Entries->Num() == 0)
		{
			QuantumPromptQueue.Remove(HighestPriority);
			return TEXT("No valid prompt available");
		}

		const FQuantumPromptQueueEntry Entry = (*Entries)[0];
		Entries->RemoveAt(0);

		if (Entries->Num() == 0)
		{
			QuantumPromptQueue.Remove(HighestPriority);
		}

		return Entry.Prompt;
	});
}

float UQuantumPromptPriorityQueueComponent::AdjustPromptPriority(float BasePriority, float Coherence, int32 HyperdimensionalWeight) const
{
	return BasePriority * (1.f + Coherence * PriorityScalingFactor) + HyperdimensionalWeight;
}

int32 UQuantumPromptPriorityQueueComponent::GenerateQuantumState() const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64());
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.RandRange(0, FMath::RoundToInt(PriorityScalingFactor * 100.f));
}

float UQuantumPromptPriorityQueueComponent::CalculateQuantumCoherence(int32 QuantumState) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) + static_cast<uint64>(QuantumState);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	return Random.FRandRange(0.f, PriorityScalingFactor) / (QuantumState + 1.f);
}

int32 UQuantumPromptPriorityQueueComponent::CalculateHyperdimensionalInfluence(int32 QuantumState, float Coherence) const
{
	return FMath::RoundToInt(QuantumState * Coherence * PriorityScalingFactor);
}
