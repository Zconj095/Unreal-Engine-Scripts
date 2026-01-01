#include "QuantumBoltzmannMachineComponent.h"
#include "NeuralLayer.h"
#include "QuantumHypervectorComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/OutputDeviceNull.h"

UQuantumBoltzmannMachineComponent::UQuantumBoltzmannMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumBoltzmannMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeNeuralNetwork();
	PerformInitialTraining();
	bIsInitialized = true;
}

void UQuantumBoltzmannMachineComponent::InitializeNeuralNetwork()
{
	if (bEnableDetailedLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Initializing neural network layers..."));
	}

	NeuralLayers.Empty();

	const int32 LayerSizes[3] = { 128, 256, 512 };
	for (int32 Index = 0; Index < 3; ++Index)
	{
		UNeuralLayer* Layer = NewObject<UNeuralLayer>(this);
		if (Layer)
		{
			Layer->Initialize(Index, LayerSizes[Index]);
			NeuralLayers.Add(Layer);
		}
	}

	if (bEnableDetailedLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Neural network initialized with %d layers."), NeuralLayers.Num());
	}
}

void UQuantumBoltzmannMachineComponent::PerformInitialTraining()
{
	if (bEnableDetailedLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Beginning initial training sequence..."));
	}

	for (UNeuralLayer* Layer : NeuralLayers)
	{
		if (Layer)
		{
			Layer->UpdateWeights();
		}
	}

	if (bEnableDetailedLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Initial training completed."));
	}
}

void UQuantumBoltzmannMachineComponent::PreloadProcessedPatterns(const TArray<FPatternData>& Patterns)
{
	NeuralLayers.Empty();
	int32 LayerSize = 128;

	for (int32 LayerIndex = 0; LayerIndex < 3; ++LayerIndex)
	{
		UNeuralLayer* Layer = NewObject<UNeuralLayer>(this);

		if (!Layer)
		{
			continue;
		}

		Layer->Initialize(LayerIndex, LayerSize);
		TArray<float> Weights;
		Weights.Reserve(LayerSize);

		for (int32 PatternIndex = 0; PatternIndex < LayerSize && PatternIndex < Patterns.Num(); ++PatternIndex)
		{
			Weights.Add(Patterns[PatternIndex].Weight);
		}

		Layer->SetWeights(Weights);
		NeuralLayers.Add(Layer);
		LayerSize *= 2;
	}
}

void UQuantumBoltzmannMachineComponent::CollapseToHypervector()
{
	if (!bIsInitialized || !HypervectorSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumBoltzmannMachine: Cannot collapse to hypervector - system not initialized or hypervector missing."));
		return;
	}

	CollapseToHypervectorInternal();
}

void UQuantumBoltzmannMachineComponent::CollapseToHypervectorInternal()
{
	if (bEnableDetailedLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Beginning state collapse to hypervectors..."));
	}

	for (UNeuralLayer* Layer : NeuralLayers)
	{
		if (Layer)
		{
			HypervectorSystem->GenerateHypervector(Layer->LayerID, Layer->GetWeights());
		}
	}

	if (bEnableDetailedLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Successfully collapsed neural state to hypervectors."));
	}
}

void UQuantumBoltzmannMachineComponent::TrainBoltzmannMachine()
{
	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumBoltzmannMachine: Cannot train - system not yet initialized!"));
		return;
	}

	for (UNeuralLayer* Layer : NeuralLayers)
	{
		if (Layer)
		{
			Layer->UpdateWeights();
		}
	}

	UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Training cycle completed."));
}

void UQuantumBoltzmannMachineComponent::ComputeHyperdriveState()
{
	if (!bIsInitialized || !HypervectorSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumBoltzmannMachine: Cannot compute hyperdrive state - system not initialized!"));
		return;
	}

	for (int32 Index = 0; Index < NeuralLayers.Num() - 1; ++Index)
	{
		if (!NeuralLayers[Index] || !NeuralLayers[Index + 1])
		{
			continue;
		}

		const float Correlation = HypervectorSystem->ComputePatternCorrelation(Index, Index + 1);
		UE_LOG(LogTemp, Log, TEXT("Hypervector Correlation between Layer %d and %d: %.4f"), Index, Index + 1, Correlation);
	}
}

void UQuantumBoltzmannMachineComponent::OverrideNeuralLayers(const TArray<UNeuralLayer*>& NewLayers)
{
	NeuralLayers = NewLayers;
	UE_LOG(LogTemp, Log, TEXT("QuantumBoltzmannMachine: Neural layers successfully reconstructed from hypervector memory."));
}

TMap<int32, TArray<float>> UQuantumBoltzmannMachineComponent::GetHebbianClusters() const
{
	TMap<int32, TArray<float>> HebbianClusters;

	if (!bIsInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("QuantumBoltzmannMachine: Cannot compute Hebbian clusters - system not yet initialized!"));
		return HebbianClusters;
	}

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

	return HebbianClusters;
}

void UQuantumBoltzmannMachineComponent::UpdateTreeSizeAnalysis(const FString& FileName, const FString& Size, const FString& LastAccessed)
{
	if (!TreeSizeData.Contains(FileName))
	{
		TreeSizeData.Add(FileName, { Size, LastAccessed });
		if (bEnableDetailedLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("TreeSize Analysis Updated: %s | Size: %s | Last Accessed: %s"), *FileName, *Size, *LastAccessed);
		}
	}
}

void UQuantumBoltzmannMachineComponent::DisplayTreeSizeReport() const
{
	UE_LOG(LogTemp, Log, TEXT("=== TreeSize Professional Report ==="));
	for (const auto& Pair : TreeSizeData)
	{
		UE_LOG(LogTemp, Log, TEXT("File: %s | Size: %s | Last Accessed: %s"), *Pair.Key, *Pair.Value.Size, *Pair.Value.LastAccessed);
	}
}

int32 UQuantumBoltzmannMachineComponent::GetProcessedFileCount() const
{
	return ProcessedFiles.Num();
}

void UQuantumBoltzmannMachineComponent::AnalyzeTextFile(const FString& FileName) const
{
	if (const TArray<FString>* Patterns = ProcessedFiles.Find(FileName))
	{
		UE_LOG(LogTemp, Log, TEXT("Pattern Analysis for %s:"), *FileName);
		for (const FString& Pattern : *Patterns)
		{
			UE_LOG(LogTemp, Log, TEXT("- %s"), *Pattern);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("File '%s' has not been processed."), *FileName);
	}
}

int32 UQuantumBoltzmannMachineComponent::KMeansCluster(const TArray<float>& Weights) const
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
		const int32 RandomIndex = FMath::RandRange(0, Weights.Num() - 1);
		Centroids[Index] = Weights[RandomIndex];
	}

	int32 BestCluster = 0;
	float MinDistance = TNumericLimits<float>::Max();

	for (int32 WeightIndex = 0; WeightIndex < Weights.Num(); ++WeightIndex)
	{
		for (int32 ClusterIndex = 0; ClusterIndex < ClusterCount; ++ClusterIndex)
		{
			const float Distance = FMath::Abs(Weights[WeightIndex] - Centroids[ClusterIndex]);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				BestCluster = ClusterIndex;
			}
		}
	}

	return BestCluster;
}
