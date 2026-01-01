#include "DistributedEncodingManager.h"
#include "Math/UnrealMathUtility.h"

FHypervector::FHypervector(int32 Dimension)
{
	if (Dimension <= 0)
	{
		return;
	}

	Values.SetNum(Dimension);
	for (int32 Index = 0; Index < Dimension; ++Index)
	{
		Values[Index] = FMath::FRandRange(-1.0f, 1.0f);
	}
}

FHypervector FHypervector::Chain(const TArray<FHypervector>& Vectors)
{
	FHypervector Result;

	if (Vectors.Num() == 0 || !Vectors[0].HasValues())
	{
		return Result;
	}

	const int32 Dimension = Vectors[0].Values.Num();
	Result.Values.SetNumZeroed(Dimension);

	for (const FHypervector& Vector : Vectors)
	{
		if (Vector.Values.Num() != Dimension)
		{
			continue;
		}

		for (int32 Index = 0; Index < Dimension; ++Index)
		{
			Result.Values[Index] += Vector.Values[Index];
		}
	}

	FString DebugValues;
	const int32 SampleCount = FMath::Min(Result.Values.Num(), 10);
	for (int32 Index = 0; Index < SampleCount; ++Index)
	{
		DebugValues += FString::Printf(TEXT("%.3f,"), Result.Values[Index]);
	}
	if (!DebugValues.IsEmpty())
	{
		UE_LOG(LogTemp, Verbose, TEXT("Chained Vector samples: %s"), *DebugValues);
	}

	return Result;
}

FHypervector FHypervector::AddNoise(const FHypervector& Source, float NoiseLevel)
{
	FHypervector Result = Source;

	if (!Result.HasValues() || NoiseLevel <= 0.0f)
	{
		return Result;
	}

	for (float& Value : Result.Values)
	{
		Value += FMath::FRandRange(-NoiseLevel, NoiseLevel);
	}

	FString DebugValues;
	const int32 SampleCount = FMath::Min(Result.Values.Num(), 5);
	for (int32 Index = 0; Index < SampleCount; ++Index)
	{
		DebugValues += FString::Printf(TEXT("Orig %.3f -> Noisy %.3f; "), Source.Values[Index], Result.Values[Index]);
	}
	if (!DebugValues.IsEmpty())
	{
		UE_LOG(LogTemp, Verbose, TEXT("AddNoise samples: %s"), *DebugValues);
	}

	return Result;
}

TArray<float> FHypervector::CalculateChanges(const FHypervector& A, const FHypervector& B)
{
	TArray<float> Delta;

	if (!A.HasValues() || !B.HasValues() || A.Values.Num() != B.Values.Num())
	{
		return Delta;
	}

	const int32 Dimension = A.Values.Num();
	Delta.SetNumZeroed(Dimension);

	for (int32 Index = 0; Index < Dimension; ++Index)
	{
		Delta[Index] = B.Values[Index] - A.Values[Index];
	}

	FString DebugValues;
	const int32 SampleCount = FMath::Min(Delta.Num(), 10);
	for (int32 Index = 0; Index < SampleCount; ++Index)
	{
		DebugValues += FString::Printf(TEXT("%.3f,"), Delta[Index]);
	}
	if (!DebugValues.IsEmpty())
	{
		UE_LOG(LogTemp, Verbose, TEXT("Changes samples: %s"), *DebugValues);
	}

	return Delta;
}

float FHypervector::CalculateSimilarity(const FHypervector& A, const FHypervector& B)
{
	if (!A.HasValues() || !B.HasValues() || A.Values.Num() != B.Values.Num())
	{
		return 0.0f;
	}

	float Dot = 0.0f;
	float NormA = 0.0f;
	float NormB = 0.0f;

	for (int32 Index = 0; Index < A.Values.Num(); ++Index)
	{
		Dot += A.Values[Index] * B.Values[Index];
		NormA += A.Values[Index] * A.Values[Index];
		NormB += B.Values[Index] * B.Values[Index];
	}

	NormA = FMath::Sqrt(NormA);
	NormB = FMath::Sqrt(NormB);

	if (NormA <= KINDA_SMALL_NUMBER || NormB <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	const float Similarity = Dot / (NormA * NormB);
	UE_LOG(LogTemp, Verbose, TEXT("CalculateSimilarity: %.3f"), Similarity);
	return Similarity;
}

ADistributedEncodingManager::ADistributedEncodingManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADistributedEncodingManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeVectors();
}

void ADistributedEncodingManager::InitializeVectors()
{
	if (NumVectors <= 0 || VectorDimension <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DistributedEncodingManager: Invalid configuration (%d vectors, dimension %d)."), NumVectors, VectorDimension);
		return;
	}

	OriginalVectors.Empty();
	OriginalVectors.Reserve(NumVectors);

	for (int32 Index = 0; Index < NumVectors; ++Index)
	{
		OriginalVectors.Add(FHypervector(VectorDimension));
	}

	ChainedOriginal = FHypervector::Chain(OriginalVectors);

	TArray<FHypervector> NoisyVectors;
	NoisyVectors.Reserve(OriginalVectors.Num());
	for (const FHypervector& Vector : OriginalVectors)
	{
		NoisyVectors.Add(FHypervector::AddNoise(Vector, NoiseLevel));
	}

	ChainedNoisy = FHypervector::Chain(NoisyVectors);

	const TArray<float> PositionalChanges = FHypervector::CalculateChanges(ChainedOriginal, ChainedNoisy);

	OutputStatistics(PositionalChanges);
}

void ADistributedEncodingManager::OutputStatistics(const TArray<float>& Changes) const
{
	if (Changes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DistributedEncodingManager: No changes to report."));
		return;
	}

	const int32 PreviewCount = FMath::Min(10, Changes.Num());
	FString Preview;
	for (int32 Index = 0; Index < PreviewCount; ++Index)
	{
		Preview += FString::SanitizeFloat(Changes[Index]);
		if (Index + 1 < PreviewCount)
		{
			Preview += TEXT(", ");
		}
	}

	UE_LOG(LogTemp, Log, TEXT("All Changes Preview (%d): %s"), PreviewCount, *Preview);

	const float MeanChange = Mean(Changes);
	const float MaxChange = Changes.Max();
	const float* MinPtr = Algo::MinElement(Changes);
	const float MinChange = MinPtr ? *MinPtr : 0.0f;

	UE_LOG(LogTemp, Log, TEXT("Mean Change: %.6f"), MeanChange);
	UE_LOG(LogTemp, Log, TEXT("Max Change: %.6f"), MaxChange);
	UE_LOG(LogTemp, Log, TEXT("Min Change: %.6f"), MinChange);

	const float ScaleChange = CalculateScaleChange(ChainedOriginal, ChainedNoisy);
	UE_LOG(LogTemp, Log, TEXT("Scale Change: %.6f"), ScaleChange);

	const float VersatilityChange = CalculateVersatilityChange();
	UE_LOG(LogTemp, Log, TEXT("Vector Versatility Change: %.6f"), VersatilityChange);
}

float ADistributedEncodingManager::Mean(const TArray<float>& Values) const
{
	if (Values.Num() == 0)
	{
		return 0.0f;
	}

	float Sum = 0.0f;
	for (float Value : Values)
	{
		Sum += Value;
	}

	UE_LOG(LogTemp, Verbose, TEXT("DistributedEncodingManager::Mean sum=%f count=%d"), Sum, Values.Num());
	return Sum / Values.Num();
}

float ADistributedEncodingManager::CalculateScaleChange(const FHypervector& A, const FHypervector& B) const
{
	const float NormA = Norm(A);
	const float NormB = Norm(B);
	const float Difference = FMath::Abs(NormA - NormB);

	UE_LOG(LogTemp, Log, TEXT("Norm A: %.6f, Norm B: %.6f, Difference: %.6f"), NormA, NormB, Difference);
	return Difference;
}

float ADistributedEncodingManager::CalculateVersatilityChange() const
{
	if (OriginalVectors.Num() == 0)
	{
		return 0.0f;
	}

	float TotalChange = 0.0f;
	for (const FHypervector& Vector : OriginalVectors)
	{
		const FHypervector Noisy = FHypervector::AddNoise(Vector, NoiseLevel);
		const TArray<float> Changes = FHypervector::CalculateChanges(Vector, Noisy);
		TotalChange += Mean(Changes);
	}

	return TotalChange / OriginalVectors.Num();
}

float ADistributedEncodingManager::Norm(const FHypervector& Vector) const
{
	if (!Vector.HasValues())
	{
		return 0.0f;
	}

	float Sum = 0.0f;
	for (float Value : Vector.Values)
	{
		Sum += Value * Value;
	}

	return FMath::Sqrt(Sum);
}
