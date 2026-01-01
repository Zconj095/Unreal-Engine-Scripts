#include "QuantumPatternAnalyzerComponent.h"
#include "QuantumHypervectorComponent.h"
#include "QuantumStorageSystemComponent.h"
#include "Async/Async.h"
#include "Kismet/KismetMathLibrary.h"

UQuantumPatternAnalyzerComponent::UQuantumPatternAnalyzerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumPatternAnalyzerComponent::BeginPlay()
{
	Super::BeginPlay();

	Hypervector = Cast<UQuantumHypervectorComponent>(GetOwner()->GetComponentByClass(UQuantumHypervectorComponent::StaticClass()));
	StorageSystem = Cast<UQuantumStorageSystemComponent>(GetOwner()->GetComponentByClass(UQuantumStorageSystemComponent::StaticClass()));

	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		InitializeAnalyzer();
	});
}

void UQuantumPatternAnalyzerComponent::InitializeAnalyzer()
{
	if (!Hypervector)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumPatternAnalyzer: Missing Hypervector."));
		return;
	}

	if (!Hypervector->IsReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumPatternAnalyzer: Hypervector not ready yet."));
		return;
	}

	ExtractHypervectorsFromState();
	PerformFeatureExtraction();
	AnalyzeTreeSizeData();
	bInitialized = true;
	UE_LOG(LogTemp, Log, TEXT("QuantumPatternAnalyzer: Initialization complete."));
}

void UQuantumPatternAnalyzerComponent::ExtractHypervectorsFromState()
{
	if (!Hypervector)
	{
		return;
	}

	const TMap<int32, TArray<float>>& HyperData = Hypervector->GetAllHypervectors();
	if (HyperData.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuantumPatternAnalyzer: No hypervectors available."));
		return;
	}

	ExtractedHypervectors = HyperData;
	UE_LOG(LogTemp, Log, TEXT("QuantumPatternAnalyzer: Extracted %d hypervectors."), ExtractedHypervectors.Num());
}

void UQuantumPatternAnalyzerComponent::PerformFeatureExtraction()
{
	for (const auto& Pair : ExtractedHypervectors)
	{
		const int32 VectorID = Pair.Key;
		const TArray<float>& Data = Pair.Value;

		const float Mean = ComputeMean(Data);
		const float Variance = ComputeVariance(Data, Mean);
		const float Entropy = ComputeEntropy(Data);

		UE_LOG(LogTemp, Log, TEXT("Hypervector %d: Mean=%.4f Variance=%.4f Entropy=%.4f"), VectorID, Mean, Variance, Entropy);
	}
}

void UQuantumPatternAnalyzerComponent::AnalyzeTreeSizeData()
{
	if (!StorageSystem)
	{
		return;
	}

	const TMap<FString, FTreeSizeRecord> TreeSizeResults = StorageSystem->GetTreeSizeData();
	for (const auto& Pair : TreeSizeResults)
	{
		UE_LOG(LogTemp, Log, TEXT("TreeSize data - %s | Size=%s | LastAccessed=%s"), *Pair.Key, *Pair.Value.Size, *Pair.Value.LastAccessed);
	}
}

void UQuantumPatternAnalyzerComponent::PreInitializeWithPatterns(const TArray<FPatternData>& Patterns)
{
	ExtractedHypervectors.Empty();

	for (int32 Index = 0; Index < Patterns.Num(); Index += 128)
	{
		TArray<float> Vector;
		for (int32 SubIndex = 0; SubIndex < 128 && (Index + SubIndex) < Patterns.Num(); ++SubIndex)
		{
			Vector.Add(Patterns[Index + SubIndex].QuantumState);
		}
		ExtractedHypervectors.Add(Index / 128, Vector);
	}

	bInitialized = true;
	UE_LOG(LogTemp, Log, TEXT("QuantumPatternAnalyzer: Pre-initialized with quantum patterns."));
}

float UQuantumPatternAnalyzerComponent::ComputeMean(const TArray<float>& Values) const
{
	if (Values.Num() == 0)
	{
		return 0.f;
	}

	float Sum = 0.f;
	for (float Value : Values)
	{
		Sum += Value;
	}

	return Sum / Values.Num();
}

float UQuantumPatternAnalyzerComponent::ComputeVariance(const TArray<float>& Values, float Mean) const
{
	if (Values.Num() == 0)
	{
		return 0.f;
	}

	float Sum = 0.f;
	for (float Value : Values)
	{
		const float Diff = Value - Mean;
		Sum += Diff * Diff;
	}

	return Sum / Values.Num();
}

float UQuantumPatternAnalyzerComponent::ComputeEntropy(const TArray<float>& Values) const
{
	if (Values.Num() == 0)
	{
		return 0.f;
	}

	float SumAbs = 0.f;
	for (float Value : Values)
	{
		SumAbs += FMath::Abs(Value);
	}

	if (SumAbs <= KINDA_SMALL_NUMBER)
	{
		return 0.f;
	}

	float Entropy = 0.f;
	for (float Value : Values)
	{
		const float P = FMath::Abs(Value) / SumAbs;
		if (P > 0.f)
		{
			Entropy -= P * FMath::Loge(P);
		}
	}

	return Entropy;
}
