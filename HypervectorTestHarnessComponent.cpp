#include "HypervectorTestHarnessComponent.h"
#include "Math/UnrealMathUtility.h"

UHypervectorTestHarnessComponent::UHypervectorTestHarnessComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHypervectorTestHarnessComponent::BeginPlay()
{
	Super::BeginPlay();

	RunHarness();
}

void UHypervectorTestHarnessComponent::RunHarness()
{
	TArray<FHypervector> Vectors;
	Vectors.Reserve(NumVectors);

	for (int32 Index = 0; Index < NumVectors; ++Index)
	{
		Vectors.Emplace(VectorDimension);
	}

	UE_LOG(LogTemp, Log, TEXT("Step 1: Generated %d hypervectors"), Vectors.Num());

	TArray<FHypervector> NoisyVectors;
	NoisyVectors.Reserve(Vectors.Num());
	for (const FHypervector& Vector : Vectors)
	{
		NoisyVectors.Add(FHypervector::AddNoise(Vector, NoiseLevel));
	}

	UE_LOG(LogTemp, Log, TEXT("Step 2: Added noise"));

	TArray<FHypervectorPoint> Points;
	Points.Reserve(Vectors.Num());
	for (const FHypervector& Vector : Vectors)
	{
		Points.Add(ToHypervectorPoint(Vector));
	}

	UHypervectorClusterComponent* ClusterComponent = NewObject<UHypervectorClusterComponent>(this);
	const TArray<FHypervectorClusterResult> Clusters = ClusterComponent->ClusterHypervectors(Points, NumClusters);

	UE_LOG(LogTemp, Log, TEXT("Step 3: Grouped into %d clusters"), Clusters.Num());
	for (const FHypervectorClusterResult& Result : Clusters)
	{
		LogValues(FString::Printf(TEXT("Cluster %d centroid snapshot"), Result.ClusterIndex + 1), Result.Centroid.Values);
	}

	if (Vectors.Num() >= 2)
	{
		const FMultidimensionalHypervector Multi0 = ToMultidimensional(Vectors[0]);
		const FMultidimensionalHypervector Multi1 = ToMultidimensional(Vectors[1]);

		const FMultidimensionalHypervector AndResult = UHypervectorGateLibrary::HypervectorAND(Multi0, Multi1);
		LogValues(TEXT("AND gate result"), AndResult.Values);

		const FMultidimensionalHypervector OrResult = UHypervectorGateLibrary::HypervectorOR(Multi0, Multi1);
		LogValues(TEXT("OR gate result"), OrResult.Values);

		const TArray<FHypervector> ChainInput = { Vectors[0], Vectors[1] };
		const FHypervector Cross = FHypervector::Chain(ChainInput);
		LogValues(TEXT("Cross dimension vector (first pair)"), Cross.Values);

		const float Rift = ComputeRift(Vectors[0], Vectors[1]);
		UE_LOG(LogTemp, Log, TEXT("Step 5: Rift = %.4f"), Rift);
	}

	UE_LOG(LogTemp, Log, TEXT("Processing complete"));
}

void UHypervectorTestHarnessComponent::LogValues(const FString& Label, const TArray<float>& Values, int32 PreviewCount) const
{
	FString Joined;
	const int32 Count = FMath::Min(PreviewCount, Values.Num());
	for (int32 Index = 0; Index < Count; ++Index)
	{
		Joined += FString::Printf(TEXT("%.3f"), Values[Index]);
		if (Index + 1 < Count)
		{
			Joined += TEXT(", ");
		}
	}
	UE_LOG(LogTemp, Log, TEXT("%s: %s..."), *Label, *Joined);
}

FHypervectorPoint UHypervectorTestHarnessComponent::ToHypervectorPoint(const FHypervector& Vector) const
{
	FHypervectorPoint Point;
	Point.Values = Vector.Values;
	return Point;
}

FMultidimensionalHypervector UHypervectorTestHarnessComponent::ToMultidimensional(const FHypervector& Vector) const
{
	FMultidimensionalHypervector Multi;
	Multi.Values = Vector.Values;
	return Multi;
}

float UHypervectorTestHarnessComponent::ComputeRift(const FHypervector& A, const FHypervector& B) const
{
	if (!A.HasValues() || !B.HasValues() || A.Values.Num() != B.Values.Num())
	{
		return 0.0f;
	}

	float Sum = 0.0f;
	for (int32 Index = 0; Index < A.Values.Num(); ++Index)
	{
		Sum += FMath::Abs(A.Values[Index] - B.Values[Index]);
	}

	return Sum;
}
