#include "QuantumHypervectorComponent.h"
#include "Math/UnrealMathUtility.h"

UQuantumHypervectorComponent::UQuantumHypervectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuantumHypervectorComponent::GenerateHypervector(int32 LayerID, const TArray<float>& NeuralWeights)
{
	TArray<float> Hypervector;
	Hypervector.SetNum(NeuralWeights.Num());

	for (int32 Index = 0; Index < NeuralWeights.Num(); ++Index)
	{
		Hypervector[Index] = FMath::Sin(NeuralWeights[Index] * PI);
	}

	HypervectorMemory.Add(LayerID, MoveTemp(Hypervector));
	UE_LOG(LogTemp, Log, TEXT("Generated Hypervector for Layer %d"), LayerID);
}

void UQuantumHypervectorComponent::PreloadQuantumStates(const TArray<FPatternData>& Patterns)
{
	HyperData.Empty();
	int32 VectorSize = 128;

	for (int32 LayerIndex = 0; LayerIndex < 3; ++LayerIndex)
	{
		TArray<float> Vector;
		Vector.Reserve(VectorSize);
		for (int32 SampleIndex = 0; SampleIndex < VectorSize && SampleIndex < Patterns.Num(); ++SampleIndex)
		{
			Vector.Add(Patterns[SampleIndex].Correlation);
		}

		HyperData.Add(LayerIndex, MoveTemp(Vector));
		VectorSize *= 2;
	}

	bIsInitialized = true;
}

TArray<float> UQuantumHypervectorComponent::GetHypervector(int32 LayerID) const
{
	if (const TArray<float>* Hypervector = HypervectorMemory.Find(LayerID))
	{
		return *Hypervector;
	}

	return TArray<float>();
}

float UQuantumHypervectorComponent::ComputePatternCorrelation(int32 LayerID1, int32 LayerID2) const
{
	const TArray<float>* HV1 = HypervectorMemory.Find(LayerID1);
	const TArray<float>* HV2 = HypervectorMemory.Find(LayerID2);

	if (!HV1 || !HV2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing hypervectors for layers %d and %d"), LayerID1, LayerID2);
		return 0.f;
	}

	const int32 Length = FMath::Min(HV1->Num(), HV2->Num());
	if (Length == 0)
	{
		return 0.f;
	}

	float Dot = 0.f;
	float Mag1 = 0.f;
	float Mag2 = 0.f;

	for (int32 Index = 0; Index < Length; ++Index)
	{
		Dot += (*HV1)[Index] * (*HV2)[Index];
		Mag1 += (*HV1)[Index] * (*HV1)[Index];
		Mag2 += (*HV2)[Index] * (*HV2)[Index];
	}

	const float Denominator = FMath::Sqrt(FMath::Max(Mag1, KINDA_SMALL_NUMBER)) * FMath::Sqrt(FMath::Max(Mag2, KINDA_SMALL_NUMBER));
	if (Denominator <= KINDA_SMALL_NUMBER)
	{
		return 0.f;
	}

	return Dot / Denominator;
}

bool UQuantumHypervectorComponent::IsReady() const
{
	return bIsInitialized && HyperData.Num() > 0;
}

const TMap<int32, TArray<float>>& UQuantumHypervectorComponent::GetAllHypervectors() const
{
	return HyperData;
}
