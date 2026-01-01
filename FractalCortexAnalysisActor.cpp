#include "FractalCortexAnalysisActor.h"

#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

AFractalCortexAnalysisActor::AFractalCortexAnalysisActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFractalCortexAnalysisActor::BeginPlay()
{
	Super::BeginPlay();
	Fc = ComputeFractalCortexActivity();
	UE_LOG(LogTemp, Display, TEXT("Fractal Cortical Activity (F_c): %.4f"), Fc);
}

void AFractalCortexAnalysisActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Fc = ComputeFractalCortexActivity();
	UE_LOG(LogTemp, Display, TEXT("Fractal Cortical Activity (F_c): %.4f"), Fc);
}

#if WITH_EDITOR
void AFractalCortexAnalysisActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DrawDebugGrid();
}
#endif

double AFractalCortexAnalysisActor::ComputeFractalCortexActivity()
{
	const double XMin = FMath::Min(XRange.X, XRange.Y);
	const double XMax = FMath::Max(XRange.X, XRange.Y);
	const double YMin = FMath::Min(YRange.X, YRange.Y);
	const double YMax = FMath::Max(YRange.X, YRange.Y);

	return Integrate2D(XMin, XMax, YMin, YMax, [this](double X, double Y) {
		const double DerivativeX = EvaluatePartialDerivative(X, Y, true);
		const double DerivativeY = EvaluatePartialDerivative(X, Y, false);
		return DerivativeX * DerivativeY;
	});
}

double AFractalCortexAnalysisActor::EvaluateFunction(double X, double Y) const
{
	return FMath::Sin(X) * FMath::Cos(Y);
}

double AFractalCortexAnalysisActor::EvaluatePartialDerivative(double X, double Y, bool bPartialX) const
{
	constexpr double Delta = 1e-3;

	if (bPartialX)
	{
		return (EvaluateFunction(X + Delta, Y) - EvaluateFunction(X - Delta, Y)) / (2.0 * Delta);
	}

	return (EvaluateFunction(X, Y + Delta) - EvaluateFunction(X, Y - Delta)) / (2.0 * Delta);
}

double AFractalCortexAnalysisActor::Integrate2D(double XMin, double XMax, double YMin, double YMax, TFunctionRef<double(double, double)> Func) const
{
	const int32 Resolution = FMath::Max(GridResolution, 2);
	const double DX = (XMax - XMin) / Resolution;
	const double DY = (YMax - YMin) / Resolution;
	double Result = 0.0;

	for (int32 I = 0; I < Resolution; ++I)
	{
		const double X = XMin + (I + 0.5) * DX;
		for (int32 J = 0; J < Resolution; ++J)
		{
			const double Y = YMin + (J + 0.5) * DY;
			Result += Func(X, Y) * DX * DY;
		}
	}

	return Result;
}

void AFractalCortexAnalysisActor::DrawDebugGrid() const
{
	if (!bDrawDebugGrid)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Base = GetActorLocation();
	const double XMin = FMath::Min(XRange.X, XRange.Y);
	const double XMax = FMath::Max(XRange.X, XRange.Y);
	const double YMin = FMath::Min(YRange.X, YRange.Y);
	const double YMax = FMath::Max(YRange.X, YRange.Y);
	const int32 Resolution = FMath::Max(GridResolution, 2);
	const double DX = (XMax - XMin) / Resolution;
	const double DY = (YMax - YMin) / Resolution;

	for (int32 I = 0; I < Resolution; ++I)
	{
		const double X = XMin + (I + 0.5) * DX;
		for (int32 J = 0; J < Resolution; ++J)
		{
			const double Y = YMin + (J + 0.5) * DY;
			const FVector Location = Base + FVector(X, Y, 0.0);
			DrawDebugPoint(World, Location, DebugPointSize, DebugPointColor, false, -1.0f, 0);
		}
	}
}
