#include "LeylineEnergyFlowUtility.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

namespace
{
	double EvaluateEnergyDensity(const FVector& Position)
	{
		const double Distance = Position.Size();
		return FMath::Exp(-Distance);
	}

	FVector EvaluateGradientLeyline(const FVector& /*Position*/)
	{
		FVector Gradient(1.0, -1.0, 0.5);
		return Gradient.GetSafeNormal();
	}
}

double ULeylineEnergyFlowUtility::ComputeLeylineEnergyFlow(const FVector& BoundsMin, const FVector& BoundsMax, int32 Steps)
{
	if (Steps <= 0)
	{
		return 0.0;
	}

	const double Hx = (BoundsMax.X - BoundsMin.X) / Steps;
	const double Hy = (BoundsMax.Y - BoundsMin.Y) / Steps;
	const double Hz = (BoundsMax.Z - BoundsMin.Z) / Steps;

	double Sum = 0.0;

	for (int32 I = 0; I <= Steps; ++I)
	{
		for (int32 J = 0; J <= Steps; ++J)
		{
			for (int32 K = 0; K <= Steps; ++K)
			{
				const double X = BoundsMin.X + I * Hx;
				const double Y = BoundsMin.Y + J * Hy;
				const double Z = BoundsMin.Z + K * Hz;

				const FVector Position(X, Y, Z);
				const double Phi = EvaluateEnergyDensity(Position);
				const FVector Gradient = EvaluateGradientLeyline(Position);
				const double Weight = ((I == 0 || I == Steps) ? 0.5 : 1.0) *
				                      ((J == 0 || J == Steps) ? 0.5 : 1.0) *
				                      ((K == 0 || K == Steps) ? 0.5 : 1.0);

				Sum += Weight * Phi * Gradient.Size();
			}
		}
	}

	return Sum * Hx * Hy * Hz;
}

void ULeylineEnergyFlowUtility::Test()
{
	const FVector BoundsMin(-10.0, -10.0, -10.0);
	const FVector BoundsMax(10.0, 10.0, 10.0);
	const int32 Steps = 10;

	const double EnergyFlow = ComputeLeylineEnergyFlow(BoundsMin, BoundsMax, Steps);
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Flow: %.2f"), EnergyFlow);
}
