#include "ElementalCycleRestoration.h"
#include "Math/UnrealMathUtility.h"

double UElementalCycleRestoration::ComputeFlux(const TArray<FVector>& VolumePoints) const
{
	double Flux = 0.0;
	for (const FVector& Point : VolumePoints)
	{
		Flux += EvaluateDivergence(Point);
		Flux += EvaluateMagicContribution(Point);
	}

	return Flux;
}

double UElementalCycleRestoration::EvaluateDivergence(const FVector& Point) const
{
	const double Sum = Point.X + Point.Y + Point.Z;
	return Sum * DivergenceScale;
}

double UElementalCycleRestoration::EvaluateMagicContribution(const FVector& Point) const
{
	const double Magnitude = Point.Size();
	return FMath::Exp(-Magnitude * MagicDecay);
}

void UElementalCycleRestoration::Test()
{
	TArray<FVector> Points;
	Points.Add(FVector(1.0f, 2.0f, 3.0f));
	Points.Add(FVector(4.0f, 5.0f, 6.0f));
	Points.Add(FVector(7.0f, 8.0f, 9.0f));

	UElementalCycleRestoration* Restoration = NewObject<UElementalCycleRestoration>();
	const double Flux = Restoration->ComputeFlux(Points);
	UE_LOG(LogTemp, Log, TEXT("Computed Elemental Flux (F): %.2f"), Flux);
}
