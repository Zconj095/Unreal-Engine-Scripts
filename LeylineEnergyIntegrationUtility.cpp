#include "LeylineEnergyIntegrationUtility.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

namespace
{
	ULeylineEnergyIntegrationUtility::FScalarField GMagicalFieldStrength;
	ULeylineEnergyIntegrationUtility::FVectorField GEnergyGradient;
}

void ULeylineEnergyIntegrationUtility::Initialize(const FScalarField& FieldStrength, const FVectorField& EnergyGradient)
{
	GMagicalFieldStrength = FieldStrength;
	GEnergyGradient = EnergyGradient;
}

double ULeylineEnergyIntegrationUtility::ComputeTotalEnergy(const FVector& BoundsMin, const FVector& BoundsMax, int32 Intervals)
{
	if (!GMagicalFieldStrength || !GEnergyGradient || Intervals <= 0)
	{
		return 0.0;
	}

	double TotalEnergy = 0.0;
	const double StepX = (BoundsMax.X - BoundsMin.X) / Intervals;
	const double StepY = (BoundsMax.Y - BoundsMin.Y) / Intervals;
	const double StepZ = (BoundsMax.Z - BoundsMin.Z) / Intervals;

	for (int32 I = 0; I <= Intervals; ++I)
	{
		const double X = BoundsMin.X + I * StepX;
		for (int32 J = 0; J <= Intervals; ++J)
		{
			const double Y = BoundsMin.Y + J * StepY;
			for (int32 K = 0; K <= Intervals; ++K)
			{
				const double Z = BoundsMin.Z + K * StepZ;
				const FVector Position(X, Y, Z);
				const double FieldStrength = GMagicalFieldStrength(Position);
				const FVector Gradient = GEnergyGradient(Position);
				TotalEnergy += FieldStrength * Gradient.Size();
			}
		}
	}

	return TotalEnergy * StepX * StepY * StepZ;
}

void ULeylineEnergyIntegrationUtility::Test()
{
	auto FieldStrength = [](const FVector& Position) -> double
	{
		return FMath::Exp(-Position.SizeSquared());
	};

	auto EnergyGradient = [](const FVector& Position) -> FVector
	{
		return FVector(2.0f * Position.X, 2.0f * Position.Y, 2.0f * Position.Z);
	};

	Initialize(FieldStrength, EnergyGradient);
	const FVector BoundsMin(-1.0f, -1.0f, -1.0f);
	const FVector BoundsMax(1.0f, 1.0f, 1.0f);
	double TotalEnergy = ComputeTotalEnergy(BoundsMin, BoundsMax, 100);
	UE_LOG(LogTemp, Log, TEXT("Total Energy Transferred (P_env): %.2f"), TotalEnergy);
}
