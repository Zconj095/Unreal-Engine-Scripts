#include "LeylineEnergyVisualizer1Actor.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

ALeylineEnergyVisualizer1Actor::ALeylineEnergyVisualizer1Actor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALeylineEnergyVisualizer1Actor::BeginPlay()
{
	Super::BeginPlay();
	VisualizeField();
}

double ALeylineEnergyVisualizer1Actor::ComputeFieldStrength(const FVector& Position) const
{
	return FMath::Exp(-Position.SizeSquared());
}

FVector ALeylineEnergyVisualizer1Actor::ComputeEnergyGradient(const FVector& Position) const
{
	return FVector(2.0f * Position.X, 2.0f * Position.Y, 2.0f * Position.Z);
}

double ALeylineEnergyVisualizer1Actor::ComputeEnergyValue(const FVector& Position) const
{
	const double FieldStrength = ComputeFieldStrength(Position);
	const FVector Gradient = ComputeEnergyGradient(Position);
	return FieldStrength * Gradient.Size();
}

void ALeylineEnergyVisualizer1Actor::VisualizeField()
{
	if (!GetWorld() || IntervalsPerAxis <= 0)
	{
		return;
	}

	const double StepX = (BoundsMax.X - BoundsMin.X) / IntervalsPerAxis;
	const double StepY = (BoundsMax.Y - BoundsMin.Y) / IntervalsPerAxis;
	const double StepZ = (BoundsMax.Z - BoundsMin.Z) / IntervalsPerAxis;

	double MaxEnergy = 0.0;
	TArray<double> EnergyValues;
	EnergyValues.Reserve(IntervalsPerAxis * IntervalsPerAxis * IntervalsPerAxis);

	for (int32 I = 0; I <= IntervalsPerAxis; ++I)
	{
		const double X = BoundsMin.X + I * StepX;
		for (int32 J = 0; J <= IntervalsPerAxis; ++J)
		{
			const double Y = BoundsMin.Y + J * StepY;
			for (int32 K = 0; K <= IntervalsPerAxis; ++K)
			{
				const double Z = BoundsMin.Z + K * StepZ;
				const FVector Position(X, Y, Z);
				const double Energy = ComputeEnergyValue(Position);
				EnergyValues.Add(Energy);
				MaxEnergy = FMath::Max(MaxEnergy, Energy);
			}
		}
	}

	int32 Index = 0;
	for (int32 I = 0; I <= IntervalsPerAxis; ++I)
	{
		const double X = BoundsMin.X + I * StepX;
		for (int32 J = 0; J <= IntervalsPerAxis; ++J)
		{
			const double Y = BoundsMin.Y + J * StepY;
			for (int32 K = 0; K <= IntervalsPerAxis; ++K)
			{
				const double Z = BoundsMin.Z + K * StepZ;
				const FVector Position(X, Y, Z);
				const double Energy = EnergyValues.IsValidIndex(Index) ? EnergyValues[Index] : 0.0;
				const float Normalized = MaxEnergy > 0.0 ? static_cast<float>(Energy / MaxEnergy) : 0.0f;
				const FColor Color = FColor::MakeRedToGreenColorFromScalar(Normalized);
				DrawDebugSphere(GetWorld(), Position, PointSize, 8, Color, Duration <= 0.0f ? false : true, Duration, 0, 1.0f);
				++Index;
			}
		}
	}
}
