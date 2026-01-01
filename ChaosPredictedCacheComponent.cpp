#include "ChaosPredictedCacheComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UChaosPredictedCacheComponent::UChaosPredictedCacheComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChaosPredictedCacheComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CleanupTimerHandle, this, &UChaosPredictedCacheComponent::CleanupExpiredPrompts, 5.f, true, 5.f);
	}
}

void UChaosPredictedCacheComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	if (!ensureMsgf(GetWorld(), TEXT("ChaosPredictedCache requires a valid world to register prompts.")))
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float PredictedExpiry = PredictExpiry(Probability);

	FChaosPromptCacheEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.ExpiryTime = CurrentTime + PredictedExpiry;

	TArray<FChaosPromptCacheEntry>& Entries = QuantumCache.FindOrAdd(Category);
	Entries.Add(MoveTemp(Entry));
}

float UChaosPredictedCacheComponent::PredictExpiry(float Probability) const
{
	return BaseExpiry * (1.f + Probability * ChaosFactor);
}

void UChaosPredictedCacheComponent::CleanupExpiredPrompts()
{
	if (!GetWorld())
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	for (auto& Pair : QuantumCache)
	{
		Pair.Value.RemoveAll([CurrentTime](const FChaosPromptCacheEntry& Entry)
		{
			return CurrentTime >= Entry.ExpiryTime;
		});
	}
}
