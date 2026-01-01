// Copyright Epic Games, Inc.

#include "HybridARIMAComponent.h"

#include "Misc/Paths.h"

UHybridARIMAComponent::UHybridARIMAComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Provide a small default time series if none is set.
	if (TimeSeries.Num() == 0)
	{
		TimeSeries = {1.f, 2.f, 3.f, 5.f, 8.f, 13.f, 21.f};
	}
}

void UHybridARIMAComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bRunOnBeginPlay)
	{
		RunOptimization();
	}
}

void UHybridARIMAComponent::RunOptimization()
{
	BestError = TNumericLimits<float>::Max();
	RunGridSearch();
	RunGeneticAlgorithm(DefaultPopulationSize, DefaultGenerations);
	RunRandomSearch(DefaultRandomIterations);

	UE_LOG(LogTemp, Log, TEXT("Best Parameters: p=%d, d=%d, q=%d, amplitude=%f, iterations=%d, error=%f"),
		BestP, BestD, BestQ, BestAmplitude, BestIterations, BestError);
}

void UHybridARIMAComponent::RunGridSearch()
{
	PerformGridSearch();
}

void UHybridARIMAComponent::RunGeneticAlgorithm(int32 PopulationSize, int32 Generations)
{
	PerformGeneticAlgorithm(PopulationSize, Generations);
}

void UHybridARIMAComponent::RunRandomSearch(int32 Iterations)
{
	PerformRandomSearch(Iterations);
}

void UHybridARIMAComponent::PerformGridSearch()
{
	const TArray<float>& Actual = GetActualOrFallback();

	for (int32 P = MinP; P <= MaxP; ++P)
	{
		for (int32 D = MinD; D <= MaxD; ++D)
		{
			for (int32 Q = MinQ; Q <= MaxQ; ++Q)
			{
				for (float Amp = MinAmplitude; Amp <= MaxAmplitude + KINDA_SMALL_NUMBER; Amp += 0.1f)
				{
					for (int32 Iter = MinIterationsParam; Iter <= MaxIterationsParam; ++Iter)
					{
						const TArray<float> Predictions = GeneratePredictions(P, D, Q, Amp, Iter);
						const float Error = EvaluateModel(Actual, Predictions);

						FCandidate Candidate;
						Candidate.P = P;
						Candidate.D = D;
						Candidate.Q = Q;
						Candidate.Amplitude = Amp;
						Candidate.Iterations = Iter;
						Candidate.Error = Error;

						UpdateBest(Candidate);
					}
				}
			}
		}
	}
}

TArray<float> UHybridARIMAComponent::GeneratePredictions(int32 P, int32 D, int32 Q, float Amplitude, int32 Iterations) const
{
	const TArray<float> DiffData = PerformDifferencing(TimeSeries, D);
	return QuantumARIMAPredict(DiffData, P, Q, Amplitude, Iterations);
}

TArray<float> UHybridARIMAComponent::PerformDifferencing(const TArray<float>& Data, int32 Degree) const
{
	TArray<float> Result = Data;
	for (int32 i = 0; i < Degree; ++i)
	{
		for (int32 j = Result.Num() - 1; j > 0; --j)
		{
			Result[j] -= Result[j - 1];
		}
		if (Result.Num() > 0)
		{
			Result.RemoveAt(0);
		}
	}
	return Result;
}

TArray<float> UHybridARIMAComponent::QuantumARIMAPredict(const TArray<float>& Data, int32 AROrder, int32 MAOrder, float Amplitude, int32 Iterations) const
{
	TArray<float> Forecast;
	const int32 Total = Data.Num() + AROrder;
	Forecast.Reserve(Total);

	for (int32 i = 0; i < Total; ++i)
	{
		const float ARComp = ARComponent(Data, Forecast, AROrder);
		const float MAComp = MAComponent(Data, i, MAOrder);
		const float QuantumAdj = SimulateQuantumAdjustment(Amplitude, Iterations);

		Forecast.Add(ARComp + MAComp + QuantumAdj);
	}

	return Forecast;
}

float UHybridARIMAComponent::ARComponent(const TArray<float>& Data, const TArray<float>& Forecast, int32 Order) const
{
	float Result = 0.f;
	for (int32 i = 1; i <= Order; ++i)
	{
		const int32 Index = Forecast.Num() - i;
		if (Index >= 0 && Forecast.IsValidIndex(Index))
		{
			Result += 0.5f * Forecast[Index];
		}
	}
	return Result;
}

float UHybridARIMAComponent::MAComponent(const TArray<float>& Data, int32 CurrentIndex, int32 Order) const
{
	float Result = 0.f;
	for (int32 i = 1; i <= Order; ++i)
	{
		const int32 DataIndex = Data.Num() - i;
		if (DataIndex >= 0 && Data.IsValidIndex(DataIndex))
		{
			Result += 0.3f * Data[DataIndex];
		}
	}
	return Result;
}

float UHybridARIMAComponent::SimulateQuantumAdjustment(float Amplitude, int32 Iterations) const
{
	float Accumulated = 0.f;
	for (int32 i = 0; i < Iterations; ++i)
	{
		const float Phase = FMath::FRandRange(0.f, PI * 2.f);
		Accumulated += Amplitude * FMath::Sin(Phase);
	}
	return (Iterations > 0) ? Accumulated / static_cast<float>(Iterations) : 0.f;
}

float UHybridARIMAComponent::EvaluateModel(const TArray<float>& Actual, const TArray<float>& Predicted) const
{
	if (Actual.Num() == 0 || Predicted.Num() == 0)
	{
		return TNumericLimits<float>::Max();
	}

	const int32 Count = FMath::Min(Actual.Num(), Predicted.Num());
	float Error = 0.f;
	for (int32 i = 0; i < Count; ++i)
	{
		const float Diff = Actual[i] - Predicted[i];
		Error += Diff * Diff;
	}
	return Error / static_cast<float>(Count);
}

void UHybridARIMAComponent::PerformRandomSearch(int32 Iterations)
{
	const TArray<float>& Actual = GetActualOrFallback();

	for (int32 i = 0; i < Iterations; ++i)
	{
		const int32 P = FMath::RandRange(MinP, MaxP);
		const int32 D = FMath::RandRange(MinD, MaxD);
		const int32 Q = FMath::RandRange(MinQ, MaxQ);
		const float Amp = FMath::FRandRange(MinAmplitude, MaxAmplitude);
		const int32 QuantumIters = FMath::RandRange(MinIterationsParam, MaxIterationsParam);

		const TArray<float> Predictions = GeneratePredictions(P, D, Q, Amp, QuantumIters);
		const float Error = EvaluateModel(Actual, Predictions);

		FCandidate Candidate{P, D, Q, Amp, QuantumIters, Error};
		UpdateBest(Candidate);
	}
}

TArray<UHybridARIMAComponent::FCandidate> UHybridARIMAComponent::InitializePopulation(int32 PopulationSize) const
{
	TArray<FCandidate> Population;
	Population.Reserve(PopulationSize);
	const TArray<float>& Actual = GetActualOrFallback();

	for (int32 i = 0; i < PopulationSize; ++i)
	{
		const int32 P = FMath::RandRange(MinP, MaxP);
		const int32 D = FMath::RandRange(MinD, MaxD);
		const int32 Q = FMath::RandRange(MinQ, MaxQ);
		const float Amp = FMath::FRandRange(MinAmplitude, MaxAmplitude);
		const int32 QuantumIters = FMath::RandRange(MinIterationsParam, MaxIterationsParam);
		const TArray<float> Predictions = GeneratePredictions(P, D, Q, Amp, QuantumIters);
		const float Error = EvaluateModel(Actual, Predictions);

		Population.Add({P, D, Q, Amp, QuantumIters, Error});
	}

	return Population;
}

UHybridARIMAComponent::FCandidate UHybridARIMAComponent::Crossover(const FCandidate& Parent1, const FCandidate& Parent2) const
{
	FCandidate Offspring;
	Offspring.P = FMath::RandBool() ? Parent1.P : Parent2.P;
	Offspring.D = FMath::RandBool() ? Parent1.D : Parent2.D;
	Offspring.Q = FMath::RandBool() ? Parent1.Q : Parent2.Q;
	Offspring.Amplitude = FMath::RandBool() ? Parent1.Amplitude : Parent2.Amplitude;
	Offspring.Iterations = FMath::RandBool() ? Parent1.Iterations : Parent2.Iterations;
	return Offspring;
}

UHybridARIMAComponent::FCandidate UHybridARIMAComponent::Mutate(const FCandidate& Candidate) const
{
	FCandidate Mutated = Candidate;
	Mutated.P = FMath::Clamp(Mutated.P + FMath::RandRange(-1, 1), MinP, MaxP);
	Mutated.D = FMath::Clamp(Mutated.D + FMath::RandRange(-1, 1), MinD, MaxD);
	Mutated.Q = FMath::Clamp(Mutated.Q + FMath::RandRange(-1, 1), MinQ, MaxQ);
	Mutated.Amplitude = FMath::Clamp(Mutated.Amplitude + FMath::FRandRange(-0.1f, 0.1f), MinAmplitude, MaxAmplitude);
	Mutated.Iterations = FMath::Clamp(Mutated.Iterations + FMath::RandRange(-1, 1), MinIterationsParam, MaxIterationsParam);
	return Mutated;
}

void UHybridARIMAComponent::PerformGeneticAlgorithm(int32 PopulationSize, int32 Generations)
{
	TArray<FCandidate> Population = InitializePopulation(PopulationSize);
	const TArray<float>& Actual = GetActualOrFallback();

	for (int32 Gen = 0; Gen < Generations; ++Gen)
	{
		Population.Sort([](const FCandidate& A, const FCandidate& B) { return A.Error < B.Error; });

		TArray<FCandidate> NewPopulation;
		NewPopulation.Reserve(PopulationSize);

		// Elitism: keep top half
		const int32 Half = PopulationSize / 2;
		for (int32 i = 0; i < Half && i < Population.Num(); ++i)
		{
			NewPopulation.Add(Population[i]);
		}

		// Crossover + mutation to fill the rest
		for (int32 i = Half; i < PopulationSize; ++i)
		{
			const FCandidate& Parent1 = Population[FMath::RandRange(0, Half - 1)];
			const FCandidate& Parent2 = Population[FMath::RandRange(0, Half - 1)];

			FCandidate Offspring = Crossover(Parent1, Parent2);
			Offspring = Mutate(Offspring);

			const TArray<float> Predictions = GeneratePredictions(Offspring.P, Offspring.D, Offspring.Q, Offspring.Amplitude, Offspring.Iterations);
			Offspring.Error = EvaluateModel(Actual, Predictions);

			NewPopulation.Add(Offspring);
		}

		Population = MoveTemp(NewPopulation);

		if (Population.Num() > 0)
		{
			const float BestGenError = Population[0].Error;
			UE_LOG(LogTemp, Log, TEXT("Generation %d: Best Error = %f"), Gen, BestGenError);
			UpdateBest(Population[0]);
		}
	}

	if (Population.Num() > 0)
	{
		Population.Sort([](const FCandidate& A, const FCandidate& B) { return A.Error < B.Error; });
		UpdateBest(Population[0]);
	}
}

void UHybridARIMAComponent::UpdateBest(const FCandidate& Candidate)
{
	if (Candidate.Error < BestError)
	{
		BestError = Candidate.Error;
		BestP = Candidate.P;
		BestD = Candidate.D;
		BestQ = Candidate.Q;
		BestAmplitude = Candidate.Amplitude;
		BestIterations = Candidate.Iterations;
	}
}

const TArray<float>& UHybridARIMAComponent::GetActualOrFallback() const
{
	return (ActualData.Num() > 0) ? ActualData : TimeSeries;
}
