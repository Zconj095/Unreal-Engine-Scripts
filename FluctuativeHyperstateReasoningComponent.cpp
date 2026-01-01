#include "FluctuativeHyperstateReasoningComponent.h"

#include "InputCoreTypes.h"
#include "Logging/LogMacros.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

UFluctuativeHyperstateReasoningComponent::UFluctuativeHyperstateReasoningComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFluctuativeHyperstateReasoningComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Hyperstates.Num() == 0)
	{
		Hyperstates.Add(FHyperstateData(TEXT("Correlation"), 0.3f));
		Hyperstates.Add(FHyperstateData(TEXT("Blessing"), 0.2f));
		Hyperstates.Add(FHyperstateData(TEXT("Event Trigger"), 0.25f));
		Hyperstates.Add(FHyperstateData(TEXT("Sound Reasoning"), 0.25f));
	}
}

void UFluctuativeHyperstateReasoningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FluctuationAccumulator += DeltaTime;
	if (FluctuationAccumulator >= FluctuationFrequency)
	{
		FluctuationAccumulator = 0.0f;
		FluctuateHyperstateProbabilities();
	}

	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		if (PC->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			FHyperstateData Picked = Reason();
			UE_LOG(LogTemp, Log, TEXT("Reasoning led to decision: %s"), *Picked.Name);
		}
	}
}

void UFluctuativeHyperstateReasoningComponent::FluctuateHyperstateProbabilities()
{
	if (Hyperstates.Num() == 0)
	{
		return;
	}

	float TotalWeight = 0.0f;
	for (FHyperstateData& State : Hyperstates)
	{
		const float Delta = FMath::FRandRange(-FluctuationRate, FluctuationRate);
		State.ProbabilityWeight = FMath::Clamp(State.ProbabilityWeight + Delta, MinWeight, 1.0f);
		TotalWeight += State.ProbabilityWeight;
	}

	if (TotalWeight <= KINDA_SMALL_NUMBER)
	{
		// fallback to uniform
		const float Uniform = 1.0f / Hyperstates.Num();
		for (FHyperstateData& State : Hyperstates)
		{
			State.ProbabilityWeight = Uniform;
		}
		return;
	}

	for (FHyperstateData& State : Hyperstates)
	{
		State.ProbabilityWeight /= TotalWeight;
	}
}

FHyperstateData UFluctuativeHyperstateReasoningComponent::Reason()
{
	if (Hyperstates.Num() == 0)
	{
		return FHyperstateData();
	}

	const float RandomValue = FMath::FRand();
	float Cumulative = 0.0f;
	for (const FHyperstateData& State : Hyperstates)
	{
		Cumulative += State.ProbabilityWeight;
		if (RandomValue <= Cumulative)
		{
			return State;
		}
	}

	// Fallback to last
	return Hyperstates.Last();
}
