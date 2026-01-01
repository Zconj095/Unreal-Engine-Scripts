#include "QuantumMarkovPromptManagerComponent.h"
#include "HAL/PlatformTime.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UQuantumMarkovPromptManagerComponent::UQuantumMarkovPromptManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumMarkovPromptManagerComponent::AddPrompt(const FString& Category, const FString& Prompt, float Probability, int32 State)
{
	TArray<FQuantumMarkovPromptEntry>& Entries = QuantumCache.FindOrAdd(Category);
	FQuantumMarkovPromptEntry Entry;
	Entry.Prompt = Prompt;
	Entry.Probability = Probability;
	Entry.State = State;
	Entry.Length = PredictMaxPromptLength(Probability, State);
	Entries.Add(MoveTemp(Entry));
}

int32 UQuantumMarkovPromptManagerComponent::PredictMaxPromptLength(float Probability, int32 State) const
{
	const uint64 Seed = static_cast<uint64>(FDateTime::Now().GetTicks()) ^ static_cast<uint64>(FPlatformTime::Cycles64()) ^ static_cast<uint64>(State);
	FRandomStream Random(static_cast<int32>(Seed & 0xffffffff));
	const float ChaosShift = Random.FRandRange(0.f, QuantumInfluenceFactor);
	const int32 AdjustedLength = FMath::RoundToInt(MaxPromptLength * (Probability * ChaosShift));
	return FMath::Clamp(AdjustedLength, MinPromptLength, MaxPromptLength);
}
