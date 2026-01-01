// Copyright Epic Games, Inc.

#include "QuantumRadonAlgorithmComponent.h"

#include "Logging/LogMacros.h"

UQuantumRadonAlgorithmComponent::UQuantumRadonAlgorithmComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<float> UQuantumRadonAlgorithmComponent::ApplyRadonTransform(const TArray<float>& DensityMatrix, int32 Rows, int32 Cols, float Angle)
{
	if (Rows * Cols != DensityMatrix.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyRadonTransform: Size mismatch for density matrix."));
		return DensityMatrix;
	}

	const float CosTheta = FMath::Cos(Angle);
	const float SinTheta = FMath::Sin(Angle);

	// Rotation matrix 2x2
	TArray<float> RotationMatrix;
	RotationMatrix.SetNum(4);
	RotationMatrix[0] = CosTheta;  RotationMatrix[1] = -SinTheta;
	RotationMatrix[2] = SinTheta;  RotationMatrix[3] = CosTheta;

	// Multiply DensityMatrix (Rows x Cols) * RotationMatrix (2 x 2) assuming Cols == 2
	if (Cols != 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyRadonTransform: Expected 2 columns for density matrix; got %d."), Cols);
	}

	const TArray<float> Transformed = MultiplyMatrices(DensityMatrix, Rows, Cols, RotationMatrix, 2, 2);

	UE_LOG(LogTemp, Log, TEXT("Applied Quantum Radon Transform."));
	return Transformed;
}

TArray<float> UQuantumRadonAlgorithmComponent::MultiplyMatrices(const TArray<float>& Mat1, int32 Rows1, int32 Cols1, const TArray<float>& Mat2, int32 Rows2, int32 Cols2)
{
	if (Cols1 != Rows2)
	{
		UE_LOG(LogTemp, Error, TEXT("MultiplyMatrices: Dimension mismatch (%d x %d) * (%d x %d)."), Rows1, Cols1, Rows2, Cols2);
		return {};
	}

	TArray<float> Result;
	Result.Init(0.f, Rows1 * Cols2);

	for (int32 i = 0; i < Rows1; ++i)
	{
		for (int32 j = 0; j < Cols2; ++j)
		{
			float Sum = 0.f;
			for (int32 k = 0; k < Cols1; ++k)
			{
				Sum += Mat1[i * Cols1 + k] * Mat2[k * Cols2 + j];
			}
			Result[i * Cols2 + j] = Sum;
		}
	}

	return Result;
}
