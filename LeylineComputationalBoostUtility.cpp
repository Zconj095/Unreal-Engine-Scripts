#include "LeylineComputationalBoostUtility.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

namespace
{
	int32 FlattenIndex(int32 X, int32 Y, int32 Z, int32 GridSize)
	{
		return X + GridSize * (Y + GridSize * Z);
	}
}

void ULeylineComputationalBoostUtility::ComputeGradient(int32 GridSize, double Dx, const TArray<double>& ScalarField, TArray<FVector>& OutGradient)
{
	OutGradient.SetNumZeroed(GridSize * GridSize * GridSize);

	for (int32 X = 1; X < GridSize - 1; ++X)
	{
		for (int32 Y = 1; Y < GridSize - 1; ++Y)
		{
			for (int32 Z = 1; Z < GridSize - 1; ++Z)
			{
				const int32 Index = FlattenIndex(X, Y, Z, GridSize);
				const double GradX = (ScalarField[FlattenIndex(X + 1, Y, Z, GridSize)] - ScalarField[FlattenIndex(X - 1, Y, Z, GridSize)]) / (2.0 * Dx);
				const double GradY = (ScalarField[FlattenIndex(X, Y + 1, Z, GridSize)] - ScalarField[FlattenIndex(X, Y - 1, Z, GridSize)]) / (2.0 * Dx);
				const double GradZ = (ScalarField[FlattenIndex(X, Y, Z + 1, GridSize)] - ScalarField[FlattenIndex(X, Y, Z - 1, GridSize)]) / (2.0 * Dx);

				OutGradient[Index] = FVector(GradX, GradY, GradZ);
			}
		}
	}
}

double ULeylineComputationalBoostUtility::ComputeLeylinePower(int32 GridSize, double Dx, const TArray<double>& MagicalField, const TArray<FVector>& PotentialGradient)
{
	double Power = 0.0;
	const double VolumeElement = FMath::Pow(Dx, 3);

	for (int32 X = 1; X < GridSize - 1; ++X)
	{
		for (int32 Y = 1; Y < GridSize - 1; ++Y)
		{
			for (int32 Z = 1; Z < GridSize - 1; ++Z)
			{
				const int32 Index = FlattenIndex(X, Y, Z, GridSize);
				const double Value = MagicalField.IsValidIndex(Index) ? MagicalField[Index] : 0.0;
				const FVector Gradient = PotentialGradient.IsValidIndex(Index) ? PotentialGradient[Index] : FVector::ZeroVector;
				const double Dot = FVector::DotProduct(Gradient, Gradient);
				const double DotProduct = Value * Dot;
				Power += DotProduct * VolumeElement;
			}
		}
	}

	return Power;
}

void ULeylineComputationalBoostUtility::Test()
{
	const int32 GridSize = 10;
	const double Dx = 1.0;

	TArray<double> MagicalField;
	MagicalField.SetNumZeroed(GridSize * GridSize * GridSize);

	TArray<double> PotentialField;
	PotentialField.SetNumZeroed(GridSize * GridSize * GridSize);

	for (int32 X = 0; X < GridSize; ++X)
	{
		for (int32 Y = 0; Y < GridSize; ++Y)
		{
			for (int32 Z = 0; Z < GridSize; ++Z)
			{
				const int32 Index = FlattenIndex(X, Y, Z, GridSize);
				MagicalField[Index] = FMath::Sin(X * Dx) * FMath::Cos(Y * Dx) * FMath::Sin(Z * Dx);
				PotentialField[Index] = FMath::Cos(X * Dx) * FMath::Sin(Y * Dx) * FMath::Cos(Z * Dx);
			}
		}
	}

	TArray<FVector> Gradient;
	ComputeGradient(GridSize, Dx, PotentialField, Gradient);
	const double Power = ComputeLeylinePower(GridSize, Dx, MagicalField, Gradient);
	UE_LOG(LogTemp, Log, TEXT("Leyline Computational Power: %.6f"), Power);
}
