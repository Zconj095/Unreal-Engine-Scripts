#include "EvoAlgorithmComponent.h"
#include "Math/UnrealMathUtility.h"

FEvolutionaryNeuralNet::FEvolutionaryNeuralNet()
{
}

FEvolutionaryNeuralNet::FEvolutionaryNeuralNet(const TArray<int32>& LayerSizesInit, bool bRandomize)
    : LayerSizes(LayerSizesInit)
{
    WeightCount = CalculateWeightCount();
    if (bRandomize)
    {
        RandomizeWeights();
    }
}

int32 FEvolutionaryNeuralNet::CalculateWeightCount() const
{
    if (LayerSizes.Num() < 2)
    {
        return 0;
    }

    int32 Count = 0;
    for (int32 Index = 0; Index < LayerSizes.Num() - 1; ++Index)
    {
        Count += LayerSizes[Index] * LayerSizes[Index + 1];
    }
    return Count;
}

void FEvolutionaryNeuralNet::RandomizeWeights()
{
    if (LayerSizes.Num() < 2)
    {
        return;
    }

    WeightCount = CalculateWeightCount();
    if (WeightCount <= 0)
    {
        Weights.Reset();
        return;
    }

    Weights.SetNumZeroed(WeightCount);
    for (float& Value : Weights)
    {
        Value = FMath::FRandRange(-1.0f, 1.0f);
    }
}

UEvoAlgorithmComponent::UEvoAlgorithmComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEvoAlgorithmComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializePopulation();
}

void UEvoAlgorithmComponent::InitializePopulation()
{
    Population.Reset();

    if (NetworkStructure.Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("EvoAlgorithmComponent: Invalid network structure."));
        return;
    }

    for (int32 Index = 0; Index < PopulationSize; ++Index)
    {
        Population.Emplace(NetworkStructure, true);
    }
}

const FEvolutionaryNeuralNet& UEvoAlgorithmComponent::SelectParent()
{
    if (Population.Num() == 0)
    {
        static FEvolutionaryNeuralNet DefaultNet;
        return DefaultNet;
    }

    const int32 Index = RandomStream.RandRange(0, Population.Num() - 1);
    return Population[Index];
}

FEvolutionaryNeuralNet UEvoAlgorithmComponent::Crossover(const FEvolutionaryNeuralNet& A, const FEvolutionaryNeuralNet& B)
{
    FEvolutionaryNeuralNet Result = A;
    const int32 WeightCount = FMath::Min(A.GetWeightCount(), B.GetWeightCount());

    Result.Weights.SetNumZeroed(WeightCount);
    for (int32 Index = 0; Index < WeightCount; ++Index)
    {
        Result.Weights[Index] = (RandomStream.FRand() < 0.5f) ? A.Weights[Index] : B.Weights[Index];
    }

    return Result;
}

void UEvoAlgorithmComponent::Mutate(FEvolutionaryNeuralNet& Candidate)
{
    for (float& Value : Candidate.Weights)
    {
        if (RandomStream.FRand() < MutationChance)
        {
            Value += FMath::FRandRange(-MutationStrength, MutationStrength);
        }
    }
}

void UEvoAlgorithmComponent::EvolvePopulation()
{
    if (Population.Num() == 0)
    {
        InitializePopulation();
        return;
    }

    TArray<FEvolutionaryNeuralNet> NextGen;
    NextGen.Reserve(Population.Num());

    for (int32 Index = 0; Index < Population.Num(); ++Index)
    {
        const FEvolutionaryNeuralNet& ParentA = SelectParent();
        const FEvolutionaryNeuralNet& ParentB = SelectParent();
        FEvolutionaryNeuralNet Child = Crossover(ParentA, ParentB);
        Mutate(Child);
        NextGen.Add(MoveTemp(Child));
    }

    Population = MoveTemp(NextGen);
}
