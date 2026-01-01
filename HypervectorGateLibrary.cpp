#include "HypervectorGateLibrary.h"
#include "Math/UnrealMathUtility.h"

FMultidimensionalHypervector UHypervectorGateLibrary::HypervectorAND(const FMultidimensionalHypervector& A, const FMultidimensionalHypervector& B)
{
	FMultidimensionalHypervector Result;
	const int32 Dimensions = FMath::Min(A.Num(), B.Num());
	Result.Values.SetNumZeroed(Dimensions);

	for (int32 Index = 0; Index < Dimensions; ++Index)
	{
		Result.Values[Index] = FMath::Min(A.Values.IsValidIndex(Index) ? A.Values[Index] : 0.0f,
		                                   B.Values.IsValidIndex(Index) ? B.Values[Index] : 0.0f);
	}

	return Result;
}

FMultidimensionalHypervector UHypervectorGateLibrary::HypervectorOR(const FMultidimensionalHypervector& A, const FMultidimensionalHypervector& B)
{
	FMultidimensionalHypervector Result;
	const int32 Dimensions = FMath::Min(A.Num(), B.Num());
	Result.Values.SetNumZeroed(Dimensions);

	for (int32 Index = 0; Index < Dimensions; ++Index)
	{
		Result.Values[Index] = FMath::Max(A.Values.IsValidIndex(Index) ? A.Values[Index] : 0.0f,
		                                   B.Values.IsValidIndex(Index) ? B.Values[Index] : 0.0f);
	}

	return Result;
}
