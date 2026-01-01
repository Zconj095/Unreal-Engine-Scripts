#include "MultidimensionalHypervectorLibrary.h"
#include "Math/UnrealMathUtility.h"

FMultidimensionalHypervector UMultidimensionalHypervectorLibrary::AddNoise(const FMultidimensionalHypervector& Vector, float NoiseLevel)
{
	FMultidimensionalHypervector Result = Vector;
	const int32 Count = Vector.Num();
	Result.Values.SetNumZeroed(Count);
	for (int32 Index = 0; Index < Count; ++Index)
	{
		const float Value = Vector.Values.IsValidIndex(Index) ? Vector.Values[Index] : 0.0f;
		Result.Values[Index] = Value + FMath::FRandRange(-NoiseLevel, NoiseLevel);
	}
	return Result;
}

float UMultidimensionalHypervectorLibrary::CalculateSimilarity(const FMultidimensionalHypervector& A, const FMultidimensionalHypervector& B)
{
	if (A.Num() == 0 || B.Num() == 0)
	{
		return 0.0f;
	}

	const int32 Count = FMath::Min(A.Num(), B.Num());
	float Dot = 0.0f;
	float NormA = 0.0f;
	float NormB = 0.0f;
	for (int32 Index = 0; Index < Count; ++Index)
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
	return Dot / (NormA * NormB);
}

FMultidimensionalHypervector UMultidimensionalHypervectorLibrary::CrossDimension(const FMultidimensionalHypervector& A, const FMultidimensionalHypervector& B)
{
	const int32 Count = FMath::Max(A.Num(), B.Num());
	FMultidimensionalHypervector Result;
	Result.Values.SetNumZeroed(Count);
	for (int32 Index = 0; Index < Count; ++Index)
	{
		const float ValueA = A.Values.IsValidIndex(Index) ? A.Values[Index] : 0.0f;
		const float ValueB = B.Values.IsValidIndex(Index) ? B.Values[Index] : 0.0f;
		Result.Values[Index] = (ValueA + ValueB) * 0.5f;
	}
	return Result;
}
