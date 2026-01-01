#include "LeylineBiosynthesisUtility.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

TArray<double> ULeylineBiosynthesisUtility::GenerateMagicalField(int32 GridSize, double Dx)
{
	TArray<double> Field;
	Field.SetNumUninitialized(GridSize * GridSize * GridSize);
	FRandomStream Rand(42);

	for (int32 X = 0; X < GridSize; ++X)
	{
		for (int32 Y = 0; Y < GridSize; ++Y)
		{
			for (int32 Z = 0; Z < GridSize; ++Z)
			{
				const int32 Index = X + GridSize * (Y + GridSize * Z);
				const double Value = FMath::Sin(X * Dx) * FMath::Cos(Y * Dx) * FMath::Exp(-Z * Dx) + Rand.FRandRange(0.0, 0.1);
				Field[Index] = Value;
			}
		}
	}

	return Field;
}

void ULeylineBiosynthesisUtility::ComputeGradient(int32 GridSize, double Dx, const TArray<double>& Field, TArray<FVector>& OutGradient)
{
	OutGradient.SetNumUninitialized(GridSize * GridSize * GridSize);

	for (int32 X = 1; X < GridSize - 1; ++X)
	{
		for (int32 Y = 1; Y < GridSize - 1; ++Y)
		{
			for (int32 Z = 1; Z < GridSize - 1; ++Z)
			{
				const int32 Index = X + GridSize * (Y + GridSize * Z);
				const double GradX = (Field[Index + 1] - Field[Index - 1]) / (2.0 * Dx);
				const double GradY = (Field[Index + GridSize] - Field[Index - GridSize]) / (2.0 * Dx);
				const double GradZ = (Field[Index + GridSize * GridSize] - Field[Index - GridSize * GridSize]) / (2.0 * Dx);
				OutGradient[Index] = FVector(GradX, GradY, GradZ);
			}
		}
	}
}

double ULeylineBiosynthesisUtility::ComputeBiosyntheticEnergy(int32 GridSize, double Dx, const TArray<double>& Field, const TArray<FVector>& Gradient)
{
	double TotalEnergy = 0.0;
	const double VolumeElement = FMath::Pow(Dx, 3);

	for (int32 X = 1; X < GridSize - 1; ++X)
	{
		for (int32 Y = 1; Y < GridSize - 1; ++Y)
		{
			for (int32 Z = 1; Z < GridSize - 1; ++Z)
			{
				const int32 Index = X + GridSize * (Y + GridSize * Z);
				const FVector& VectorGradient = Gradient[Index];
				const FVector Ones(1.0f, 1.0f, 1.0f);
				const double Dot = FVector::DotProduct(VectorGradient, Ones);
				const double DotProduct = Field[Index] * Dot;
				TotalEnergy += DotProduct * VolumeElement;
			}
		}
	}

	return TotalEnergy;
}

void ULeylineBiosynthesisUtility::Test()
{
	const int32 GridSize = 10;
	const double Dx = 1.0;

	const TArray<double> Field = GenerateMagicalField(GridSize, Dx);
	TArray<FVector> Gradient;
	ComputeGradient(GridSize, Dx, Field, Gradient);

	const double TotalEnergy = ComputeBiosyntheticEnergy(GridSize, Dx, Field, Gradient);
	UE_LOG(LogTemp, Log, TEXT("Total Biosynthetic Energy: %.4f"), TotalEnergy);
}
