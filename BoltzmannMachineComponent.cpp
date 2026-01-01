#include "BoltzmannMachineComponent.h"
#include "NeuralLayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/LogMacros.h"

UBoltzmannMachineComponent::UBoltzmannMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBoltzmannMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeNeuralNetwork();
	TrainBoltzmannMachine();
}

void UBoltzmannMachineComponent::InitializeNeuralNetwork()
{
	if (NeuralLayers.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("BoltzmannMachineComponent: No neural layers assigned; skipping initialization."));
		return;
	}

	for (UNeuralLayer* Layer : NeuralLayers)
	{
		if (!Layer)
		{
			UE_LOG(LogTemp, Warning, TEXT("BoltzmannMachineComponent: Encountered null layer during initialization."));
		}
	}
}

void UBoltzmannMachineComponent::TrainBoltzmannMachine()
{
	for (UNeuralLayer* Layer : NeuralLayers)
	{
		if (Layer)
		{
			Layer->UpdateWeights();
		}
	}

	ComputeHebbianClusters();
}

const TMap<int32, TArray<float>>& UBoltzmannMachineComponent::GetHebbianClusters() const
{
	return HebbianClusters;
}

void UBoltzmannMachineComponent::ComputeHebbianClusters()
{
	HebbianClusters.Empty();

	for (UNeuralLayer* Layer : NeuralLayers)
	{
		if (!Layer)
		{
			continue;
		}

		const TArray<float> Weights = Layer->GetWeights();
		const int32 ClusterID = KMeansCluster(Weights);
		HebbianClusters.Add(ClusterID, Weights);
	}
}

int32 UBoltzmannMachineComponent::KMeansCluster(const TArray<float>& Weights) const
{
	const int32 ClusterCount = 3;
	if (Weights.Num() == 0)
	{
		return INDEX_NONE;
	}

	TArray<float> Centroids;
	Centroids.SetNum(ClusterCount);

	for (int32 Index = 0; Index < ClusterCount; ++Index)
	{
		const int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, Weights.Num() - 1);
		Centroids[Index] = Weights[RandomIndex];
	}

	int32 BestCluster = INDEX_NONE;
	float MinDistance = TNumericLimits<float>::Max();

	for (int32 WeightIndex = 0; WeightIndex < Weights.Num(); ++WeightIndex)
	{
		const float Weight = Weights[WeightIndex];
		for (int32 ClusterIndex = 0; ClusterIndex < ClusterCount; ++ClusterIndex)
		{
			const float Distance = FMath::Abs(Weight - Centroids[ClusterIndex]);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				BestCluster = ClusterIndex;
			}
		}
	}

	return BestCluster;
}
