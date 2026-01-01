#include "EvolutionaryNetworkLibrary.h"

#include "Algo/Sort.h"
#include "Math/UnrealMathUtility.h"

void UEvolutionaryNetworkLibrary::MutateNetwork(FMultidimensionalNetwork& Network)
{
	const int32 Id = FMath::RandRange(1, 100);
	const FString Key = FString::Printf(TEXT("dim%d"), Id);
	Network.Dimensions.Add(Key, FMath::FRand());
}

FMultidimensionalNetwork UEvolutionaryNetworkLibrary::CrossoverNetworks(const FMultidimensionalNetwork& ParentA, const FMultidimensionalNetwork& ParentB)
{
	FMultidimensionalNetwork Child;

	TArray<FString> Keys;
	ParentA.Dimensions.GetKeys(Keys);
	const int32 Split = Keys.Num() / 2;

	for (int32 Index = 0; Index < Keys.Num(); ++Index)
	{
		const FString& Key = Keys[Index];
		if (Index < Split)
		{
			if (const float* Val = ParentA.Dimensions.Find(Key))
			{
				Child.Dimensions.Add(Key, *Val);
			}
		}
		else
		{
			if (const float* ValB = ParentB.Dimensions.Find(Key))
			{
				Child.Dimensions.Add(Key, *ValB);
			}
			else if (const float* ValA = ParentA.Dimensions.Find(Key))
			{
				Child.Dimensions.Add(Key, *ValA);
			}
		}
	}

	return Child;
}

float UEvolutionaryNetworkLibrary::EvaluateFitness(FMultidimensionalNetwork& Network)
{
	Network.Fitness = static_cast<float>(Network.Dimensions.Num());
	return Network.Fitness;
}

void UEvolutionaryNetworkLibrary::EvolveNetworks(int32 PopulationSize, int32 Generations, FMultidimensionalNetwork& OutBestNetwork)
{
	PopulationSize = FMath::Max(1, PopulationSize);
	Generations = FMath::Max(0, Generations);

	TArray<FMultidimensionalNetwork> Population;
	Population.SetNum(PopulationSize);

	for (int32 Gen = 0; Gen < Generations; ++Gen)
	{
		// Mutate all
		for (FMultidimensionalNetwork& Net : Population)
		{
			MutateNetwork(Net);
		}

		// Crossover pairs
		TArray<FMultidimensionalNetwork> Offspring;
		for (int32 I = 0; I + 1 < Population.Num(); I += 2)
		{
			Offspring.Add(CrossoverNetworks(Population[I], Population[I + 1]));
		}
		Population.Append(Offspring);

		// Evaluate fitness
		for (FMultidimensionalNetwork& Net : Population)
		{
			EvaluateFitness(Net);
		}

		// Keep the best PopulationSize
		Algo::Sort(Population, [](const FMultidimensionalNetwork& A, const FMultidimensionalNetwork& B)
		{
			return A.Fitness > B.Fitness;
		});
		if (Population.Num() > PopulationSize)
		{
			Population.SetNum(PopulationSize);
		}
	}

	// Return best
	if (Population.Num() > 0)
	{
		OutBestNetwork = Population[0];
		EvaluateFitness(OutBestNetwork);
	}
}
