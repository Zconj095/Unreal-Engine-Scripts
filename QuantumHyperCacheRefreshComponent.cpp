#include "QuantumHyperCacheRefreshComponent.h"
#include "Engine/World.h"
#include "HAL/PlatformTime.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

UQuantumHyperCacheRefreshComponent::UQuantumHyperCacheRefreshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHyperCacheRefreshComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RefreshTimerHandle, this, &UQuantumHyperCacheRefreshComponent::ApplyQuantumCacheRefresh, BaseRefreshInterval, true);
	}
}

void UQuantumHyperCacheRefreshComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RefreshTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
}

void UQuantumHyperCacheRefreshComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability)
{
	const int32 Hyperstate = CalculateHyperstate(Probability);

	TArray<FQuantumHyperCacheRefreshEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumHyperCacheRefreshEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.Hyperstate = Hyperstate;
	Entries.Add(MoveTemp(Entry));

	AdjustCacheRefreshInterval();
}

void UQuantumHyperCacheRefreshComponent::ApplyQuantumCacheRefresh()
{
	for (auto& Pair : QuantumCache)
	{
		for (FQuantumHyperCacheRefreshEntry& Entry : Pair.Value)
		{
			const int32 Delta = FMath::RoundToInt(FMath::FRandRange(0.f, QuantumAdjustmentFactor));
			Entry.Hyperstate += Delta;
		}
	}
}

void UQuantumHyperCacheRefreshComponent::AdjustCacheRefreshInterval()
{
	if (UWorld* World = GetWorld())
	{
		const float HypervectorInfluence = FMath::FRandRange(0.f, QuantumAdjustmentFactor);
		const float AdjustedInterval = BaseRefreshInterval * (1.f + HypervectorInfluence);

		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(RefreshTimerHandle);
		TimerManager.SetTimer(RefreshTimerHandle, this, &UQuantumHyperCacheRefreshComponent::ApplyQuantumCacheRefresh, AdjustedInterval, true);
	}
}

int32 UQuantumHyperCacheRefreshComponent::CalculateHyperstate(float Probability) const
{
	const float RandomFactor = FMath::FRandRange(1.f, 10.f);
	return FMath::RoundToInt(Probability * QuantumAdjustmentFactor * RandomFactor);
}
