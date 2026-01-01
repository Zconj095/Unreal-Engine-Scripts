#include "GeneticAlgorithmManagerComponent.h"
#include "Async/Async.h"
#include "Math/UnrealMathUtility.h"

UGeneticAlgorithmManagerComponent::UGeneticAlgorithmManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGeneticAlgorithmManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializePopulation();
	RunEvolution();
}

void UGeneticAlgorithmManagerComponent::InitializePopulation()
{
	Population.Reset();
	Population.Reserve(PopulationSize);

	for (int32 Index = 0; Index < PopulationSize; ++Index)
	{
		FFloatArrayWrapper Chromosome;
		Chromosome.Values.SetNumZeroed(ChromosomeLength);
		for (int32 GeneIndex = 0; GeneIndex < ChromosomeLength; ++GeneIndex)
		{
			Chromosome.Values[GeneIndex] = FMath::FRandRange(-1.0f, 1.0f);
		}
		Population.Add(MoveTemp(Chromosome));
	}
}

void UGeneticAlgorithmManagerComponent::RunEvolution()
{
	for (Generation = 0; Generation < MaxGenerations; ++Generation)
	{
		FitnessScores = EvaluateFitness();
		AdjustParametersBasedOnFeedback();
		TArray<FFloatArrayWrapper> NextGeneration = PerformSelectionAndCrossover();
		PerformMutation(NextGeneration);
		Population = MoveTemp(NextGeneration);

		const float BestFitness = FitnessScores.Num() ? FitnessScores.Max() : 0.0f;
		UE_LOG(LogTemp, Log, TEXT("Generation %d: Best Fitness = %.4f"), Generation, BestFitness);
	}
}

TArray<float> UGeneticAlgorithmManagerComponent::EvaluateFitness() const
{
	return Async(EAsyncExecution::ThreadPool, [this]()
	{
		TArray<float> Results;
		Results.SetNumZeroed(Population.Num());

		for (int32 Index = 0; Index < Population.Num(); ++Index)
		{
			Results[Index] = FitnessFunction(Population[Index].Values);
		}

		return Results;
	}).Get();
}

void UGeneticAlgorithmManagerComponent::AdjustParametersBasedOnFeedback()
{
	const float Diversity = CalculatePopulationDiversity();
	const float BestFitness = FitnessScores.Num() ? FitnessScores.Max() : 0.0f;
	float SumFitness = 0.0f;
	for (float Value : FitnessScores)
	{
		SumFitness += Value;
	}
	const float AverageFitness = FitnessScores.Num() ? (SumFitness / FitnessScores.Num()) : 0.0f;

	if (Diversity < DiversityThreshold)
	{
		MutationRate = FMath::Clamp(MutationRate + 0.01f, 0.0f, 1.0f);
		CrossoverRate = FMath::Clamp(CrossoverRate - 0.01f, 0.0f, 1.0f);
	}
	else
	{
		MutationRate = FMath::Clamp(MutationRate - 0.01f, 0.0f, 1.0f);
		CrossoverRate = FMath::Clamp(CrossoverRate + 0.01f, 0.0f, 1.0f);
	}

	if (BestFitness - AverageFitness < FitnessImprovementThreshold)
	{
		MutationRate = FMath::Clamp(MutationRate + 0.05f, 0.0f, 1.0f);
	}
}

TArray<FFloatArrayWrapper> UGeneticAlgorithmManagerComponent::PerformSelectionAndCrossover() const
{
	TArray<FFloatArrayWrapper> NextGeneration;
	NextGeneration.Reserve(PopulationSize);

	while (NextGeneration.Num() < PopulationSize)
	{
		const FFloatArrayWrapper& ParentA = SelectParent();
		const FFloatArrayWrapper& ParentB = SelectParent();

		FFloatArrayWrapper Child = (FMath::FRand() < CrossoverRate) ? Crossover(ParentA, ParentB) : ParentA;
		NextGeneration.Add(Child);
	}

	return NextGeneration;
}

void UGeneticAlgorithmManagerComponent::PerformMutation(TArray<FFloatArrayWrapper>& NextGeneration) const
{
	for (FFloatArrayWrapper& Chromosome : NextGeneration)
	{
		for (int32 GeneIndex = 0; GeneIndex < Chromosome.Values.Num(); ++GeneIndex)
		{
			if (FMath::FRand() < MutationRate)
			{
				Chromosome.Values[GeneIndex] += FMath::FRandRange(-0.1f, 0.1f);
			}
		}
	}
}

float UGeneticAlgorithmManagerComponent::FitnessFunction(const TArray<float>& Chromosome) const
{
	float Sum = 0.0f;
	for (float Gene : Chromosome)
	{
		Sum += Gene * Gene;
	}
	return Sum;
}

float UGeneticAlgorithmManagerComponent::CalculatePopulationDiversity() const
{
	if (Population.Num() == 0)
	{
		return 0.0f;
	}

	TArray<float> MeanVector;
	MeanVector.SetNumZeroed(ChromosomeLength);

	for (const FFloatArrayWrapper& ChromosomeWrapper : Population)
	{
		for (int32 Index = 0; Index < ChromosomeLength; ++Index)
		{
			MeanVector[Index] += ChromosomeWrapper.Values.IsValidIndex(Index) ? ChromosomeWrapper.Values[Index] : 0.0f;
		}
	}

	for (float& Value : MeanVector)
	{
		Value /= Population.Num();
	}

	float Diversity = 0.0f;
	for (const FFloatArrayWrapper& ChromosomeWrapper : Population)
	{
		for (int32 Index = 0; Index < ChromosomeLength; ++Index)
		{
			const float Gene = ChromosomeWrapper.Values.IsValidIndex(Index) ? ChromosomeWrapper.Values[Index] : 0.0f;
			Diversity += FMath::Abs(Gene - MeanVector[Index]);
		}
	}

	return Diversity / Population.Num();
}

const FFloatArrayWrapper& UGeneticAlgorithmManagerComponent::SelectParent() const
{
	if (Population.Num() == 0)
	{
		static const FFloatArrayWrapper EmptyChromosome;
		return EmptyChromosome;
	}

	if (FitnessScores.Num() == 0)
	{
		return Population[FMath::RandRange(0, Population.Num() - 1)];
	}

	float TotalFitness = 0.0f;
	for (float Value : FitnessScores)
	{
		TotalFitness += Value;
	}
	TotalFitness = FMath::Max(TotalFitness, 1.0f);
	const float Threshold = FMath::FRandRange(0.0f, TotalFitness);
	float Accumulated = 0.0f;

	for (int32 Index = 0; Index < Population.Num(); ++Index)
	{
		Accumulated += FitnessScores[Index];
		if (Accumulated >= Threshold)
		{
			return Population[Index];
		}
	}

	return Population[FMath::RandRange(0, Population.Num() - 1)];
}

FFloatArrayWrapper UGeneticAlgorithmManagerComponent::Crossover(const FFloatArrayWrapper& ParentA, const FFloatArrayWrapper& ParentB) const
{
	FFloatArrayWrapper Child;
	Child.Values.SetNumZeroed(ChromosomeLength);

	for (int32 Index = 0; Index < ChromosomeLength; ++Index)
	{
		const float ValueA = ParentA.Values.IsValidIndex(Index) ? ParentA.Values[Index] : 0.0f;
		const float ValueB = ParentB.Values.IsValidIndex(Index) ? ParentB.Values[Index] : 0.0f;
		Child.Values[Index] = (FMath::FRand() < 0.5f) ? ValueA : ValueB;
	}

	return Child;
}
