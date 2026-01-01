#include "HiddenMarkovDistributionComponent.h"

#include "Logging/LogMacros.h"

UHiddenMarkovDistributionComponent::UHiddenMarkovDistributionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHiddenMarkovDistributionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeHMM();

	const TArray<int32> Observations = GenerateRandomObservations(ObservationSequenceLength);
	const TArray<int32> MostLikelyStates = Viterbi(Observations);

	FString ObsStr, StateStr;
	for (int32 i = 0; i < Observations.Num(); ++i)
	{
		ObsStr += FString::FromInt(Observations[i]);
		StateStr += FString::FromInt(MostLikelyStates.IsValidIndex(i) ? MostLikelyStates[i] : -1);
		if (i + 1 < Observations.Num())
		{
			ObsStr += TEXT(", ");
			StateStr += TEXT(", ");
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Observations: %s"), *ObsStr);
	UE_LOG(LogTemp, Log, TEXT("Most Likely States: %s"), *StateStr);
}

void UHiddenMarkovDistributionComponent::InitializeHMM()
{
	NumStates = FMath::Max(1, NumStates);
	NumObservations = FMath::Max(1, NumObservations);

	TransitionProbabilities.SetNum(NumStates * NumStates);
	EmissionProbabilities.SetNum(NumStates * NumObservations);
	InitialStateProbabilities.SetNum(NumStates);

	for (int32 i = 0; i < NumStates; ++i)
	{
		InitialStateProbabilities[i] = 1.0f / NumStates;
		for (int32 j = 0; j < NumStates; ++j)
		{
			T(i, j) = 1.0f / NumStates;
		}
		for (int32 k = 0; k < NumObservations; ++k)
		{
			E(i, k) = 1.0f / NumObservations;
		}
	}
}

TArray<int32> UHiddenMarkovDistributionComponent::GenerateRandomObservations(int32 Length) const
{
	TArray<int32> Observations;
	Observations.SetNum(Length);
	for (int32 i = 0; i < Length; ++i)
	{
		Observations[i] = FMath::RandRange(0, NumObservations - 1);
	}
	return Observations;
}

TArray<int32> UHiddenMarkovDistributionComponent::Viterbi(const TArray<int32>& Observations) const
{
	TArray<int32> Empty;
	if (Observations.Num() == 0 || NumStates == 0 || NumObservations == 0)
	{
		return Empty;
	}

	const int32 TLen = Observations.Num();
	TArray<float> DP; DP.SetNumZeroed(NumStates * TLen);
	TArray<int32> Back; Back.SetNumZeroed(NumStates * TLen);

	auto DPIdx = [&](int32 State, int32 Time) { return State * TLen + Time; };

	// Initialization
	for (int32 State = 0; State < NumStates; ++State)
	{
		const int32 Obs = Observations[0];
		if (Obs < 0 || Obs >= NumObservations) { continue; }
		DP[DPIdx(State, 0)] = InitialStateProbabilities[State] * E(State, Obs);
		Back[DPIdx(State, 0)] = 0;
	}

	// Recursion
	for (int32 t = 1; t < TLen; ++t)
	{
		const int32 Obs = Observations[t];
		if (Obs < 0 || Obs >= NumObservations) { continue; }

		for (int32 Curr = 0; Curr < NumStates; ++Curr)
		{
			float MaxProb = -1.f;
			int32 BestPrev = 0;

			for (int32 Prev = 0; Prev < NumStates; ++Prev)
			{
				const float Prob = DP[DPIdx(Prev, t - 1)] * T(Prev, Curr) * E(Curr, Obs);
				if (Prob > MaxProb)
				{
					MaxProb = Prob;
					BestPrev = Prev;
				}
			}
			DP[DPIdx(Curr, t)] = MaxProb;
			Back[DPIdx(Curr, t)] = BestPrev;
		}
	}

	// Backtracking
	TArray<int32> MostLikely; MostLikely.SetNum(TLen);
	float MaxFinal = -1.f;
	int32 LastState = 0;
	for (int32 State = 0; State < NumStates; ++State)
	{
		const float Prob = DP[DPIdx(State, TLen - 1)];
		if (Prob > MaxFinal)
		{
			MaxFinal = Prob;
			LastState = State;
		}
	}

	MostLikely[TLen - 1] = LastState;
	for (int32 t = TLen - 2; t >= 0; --t)
	{
		const int32 NextState = MostLikely[t + 1];
		MostLikely[t] = Back[DPIdx(NextState, t + 1)];
	}

	return MostLikely;
}
