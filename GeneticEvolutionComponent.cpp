#include "GeneticEvolutionComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

void FNeuralNetworkDNA::Setup(const TArray<int32>& InStructure)
{
	Structure = InStructure;
	const int32 WeightCount = GetWeightCount();
	Weights.SetNumZeroed(WeightCount);
}

int32 FNeuralNetworkDNA::GetWeightCount() const
{
	int32 Count = 0;
	if (Structure.Num() < 2)
	{
		return Count;
	}

	for (int32 Index = 0; Index + 1 < Structure.Num(); ++Index)
	{
		const int32 InputCount = FMath::Max(Structure[Index], 1);
		const int32 OutputCount = FMath::Max(Structure[Index + 1], 1);
		Count += (InputCount + 1) * OutputCount;
	}

	return Count;
}

void FNeuralNetworkDNA::RandomizeWeights(FRandomStream& Stream, float Min, float Max)
{
	for (float& Weight : Weights)
	{
		Weight = Stream.FRandRange(Min, Max);
	}
}

UGeneticEvolutionComponent::UGeneticEvolutionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	RandomStream.Initialize(static_cast<int32>(FDateTime::Now().GetTicks()));
}

void UGeneticEvolutionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializePopulation();
}

void UGeneticEvolutionComponent::InitializePopulation()
{
	if (!IsStructureValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GeneticEvolutionComponent: Invalid network structure or population size."));
		return;
	}

	Population.Reset();
	Population.Reserve(PopulationSize);

	for (int32 Index = 0; Index < PopulationSize; ++Index)
	{
		FNeuralNetworkDNA DNA;
		DNA.Setup(NetworkStructure);
		DNA.RandomizeWeights(RandomStream);
		Population.Add(MoveTemp(DNA));
	}
}

void UGeneticEvolutionComponent::EvolvePopulation()
{
	if (Population.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("GeneticEvolutionComponent: Population too small for evolution."));
		return;
	}

	TArray<FNeuralNetworkDNA> NextGen;
	NextGen.Reserve(PopulationSize);

	for (int32 Index = 0; Index < PopulationSize; ++Index)
	{
		const FNeuralNetworkDNA& ParentA = SelectParent();
		const FNeuralNetworkDNA& ParentB = SelectParent();
		FNeuralNetworkDNA Child = Crossover(ParentA, ParentB);
		Mutate(Child);
		NextGen.Add(MoveTemp(Child));
	}

	Population = MoveTemp(NextGen);
}

const FNeuralNetworkDNA& UGeneticEvolutionComponent::SelectParent() const
{
	const int32 Index = RandomStream.RandRange(0, Population.Num() - 1);
	return Population[Index];
}

FNeuralNetworkDNA UGeneticEvolutionComponent::Crossover(const FNeuralNetworkDNA& ParentA, const FNeuralNetworkDNA& ParentB)
{
	FNeuralNetworkDNA Child;
	Child.Setup(NetworkStructure);

	const int32 WeightCount = FMath::Min(ParentA.Weights.Num(), ParentB.Weights.Num());
	for (int32 Index = 0; Index < WeightCount; ++Index)
	{
		const bool bTakeFromA = (RandomStream.RandRange(0, 1) == 0);
		Child.Weights[Index] = bTakeFromA ? ParentA.Weights[Index] : ParentB.Weights[Index];
	}

	return Child;
}

void UGeneticEvolutionComponent::Mutate(FNeuralNetworkDNA& Candidate)
{
	if (MutationRatePercent <= 0)
	{
		return;
	}

	for (float& Weight : Candidate.Weights)
	{
		if (RandomStream.RandRange(0, 99) < MutationRatePercent)
		{
			Weight += RandomStream.FRandRange(-0.5f, 0.5f);
		}
	}
}

bool UGeneticEvolutionComponent::IsStructureValid() const
{
	return PopulationSize > 0 && NetworkStructure.Num() >= 2;
}
