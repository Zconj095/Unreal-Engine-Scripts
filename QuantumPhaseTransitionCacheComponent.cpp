#include "QuantumPhaseTransitionCacheComponent.h"

UQuantumPhaseTransitionCacheComponent::UQuantumPhaseTransitionCacheComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumPhaseTransitionCacheComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	TArray<FQuantumPhaseTransitionEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumPhaseTransitionEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.UsageCount = 0;
	Entries.Add(MoveTemp(Entry));

	if (Entries.Num() > MaxCacheSize)
	{
		QuantumPhaseTransition(Category);
	}
}

void UQuantumPhaseTransitionCacheComponent::QuantumPhaseTransition(const FString& Category)
{
	if (TArray<FQuantumPhaseTransitionEntry>* Entries = QuantumCache.Find(Category))
	{
		Entries->Sort([this](const FQuantumPhaseTransitionEntry& A, const FQuantumPhaseTransitionEntry& B)
		{
			const float ScoreA = A.Probability * (1.f + A.UsageCount * EntropyThreshold);
			const float ScoreB = B.Probability * (1.f + B.UsageCount * EntropyThreshold);
			return ScoreA > ScoreB;
		});

		if (Entries->Num() > MaxCacheSize)
		{
			Entries->SetNum(MaxCacheSize);
		}
	}
}
