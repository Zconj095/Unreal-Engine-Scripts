#include "HiddenMarkovModelComponent.h"

UHiddenMarkovModelComponent::UHiddenMarkovModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HiddenStates.Add({ TEXT("Sunny"), 0.25 });
	HiddenStates.Add({ TEXT("Rainy"), 0.25 });
	HiddenStates.Add({ TEXT("Cloudy"), 0.25 });
	HiddenStates.Add({ TEXT("Snowy"), 0.25 });
}

void UHiddenMarkovModelComponent::UpdateStateBasedOnWeather(const FString& Weather)
{
	if (Weather.Equals(TEXT("Rainy"), ESearchCase::IgnoreCase))
	{
		for (FHiddenMarkovState& State : HiddenStates)
		{
			if (State.StateName == TEXT("Rainy"))
			{
				State.Probability += 0.1;
			}
			else if (State.StateName == TEXT("Sunny"))
			{
				State.Probability -= 0.1;
			}
		}
	}
	else if (Weather.Equals(TEXT("Sunny"), ESearchCase::IgnoreCase))
	{
		for (FHiddenMarkovState& State : HiddenStates)
		{
			if (State.StateName == TEXT("Sunny"))
			{
				State.Probability += 0.1;
			}
			else if (State.StateName == TEXT("Rainy"))
			{
				State.Probability -= 0.1;
			}
		}
	}

	NormalizeProbabilities();
}

void UHiddenMarkovModelComponent::AdvanceTime()
{
	UE_LOG(LogTemp, Log, TEXT("Advancing HMM states for time progression."));
	// TODO: Add time-of-day effects here.
}

void UHiddenMarkovModelComponent::AdjustForSeason(const FString& Season)
{
	UE_LOG(LogTemp, Log, TEXT("Adjusting HMM for season: %s"), *Season);
	// TODO: Add seasonal adjustments here.
}

void UHiddenMarkovModelComponent::NormalizeProbabilities()
{
	double Total = 0.0;
	for (const FHiddenMarkovState& State : HiddenStates)
	{
		Total += State.Probability;
	}

	if (Total <= 0.0)
	{
		return;
	}

	for (FHiddenMarkovState& State : HiddenStates)
	{
		State.Probability /= Total;
	}
}
