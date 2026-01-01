#include "IntensitySpectrumVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/CollisionProfile.h"
#include "Math/UnrealMathUtility.h"

FElementalCompositionAnalysisData::FElementalCompositionAnalysisData(int32 InSampleSize, double InDx)
	: SampleSize(InSampleSize)
	, Dx(InDx)
{
}

TArray<double> FElementalCompositionAnalysisData::GenerateElementalDensity() const
{
	TArray<double> Density;
	Density.SetNum(SampleSize);

	for (int32 Index = 0; Index < SampleSize; ++Index)
	{
		const double X = Index * Dx;
		Density[Index] = FMath::Sin(2.0 * PI * X) + 0.5 * FMath::Cos(5.0 * PI * X);
	}

	return Density;
}

TArray<double> FElementalCompositionAnalysisData::ComputeIntensity(const TArray<double>& Density) const
{
	TArray<double> Intensity;
	if (Density.Num() == 0)
	{
		return Intensity;
	}

	const int32 N = Density.Num();
	Intensity.SetNum(N);

	for (int32 K = 0; K < N; ++K)
	{
		double Real = 0.0;
		double Imag = 0.0;
		for (int32 NIndex = 0; NIndex < N; ++NIndex)
		{
			const double Angle = -2.0 * PI * K * NIndex / N;
			Real += Density[NIndex] * FMath::Cos(Angle);
			Imag += Density[NIndex] * FMath::Sin(Angle);
		}

		Intensity[K] = FMath::Sqrt(Real * Real + Imag * Imag);
	}

	return Intensity;
}

//////////////////////////////////////////////////////////////////////////
// AIntensitySpectrumVisualizerActor

AIntensitySpectrumVisualizerActor::AIntensitySpectrumVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	BarInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BarInstances"));
	BarInstances->SetupAttachment(RootComponent);
	BarInstances->SetMobility(EComponentMobility::Movable);
	BarInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void AIntensitySpectrumVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	Analyzer = FElementalCompositionAnalysisData(SampleSize, Dx);
	GenerateAndVisualizeIntensity();
}

void AIntensitySpectrumVisualizerActor::GenerateAndVisualizeIntensity()
{
	Density = Analyzer.GenerateElementalDensity();
	Intensity = Analyzer.ComputeIntensity(Density);

	LogIntensitySummary();

	if (BarMesh)
	{
		BarInstances->SetStaticMesh(BarMesh);
		if (BarMaterial)
		{
			BarInstances->SetMaterial(0, BarMaterial);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BarMesh is not configured; skipping visualization."));
	}

	VisualizeIntensityBars();
}

void AIntensitySpectrumVisualizerActor::VisualizeIntensityBars()
{
	if (!BarMesh || Intensity.Num() == 0)
	{
		return;
	}

	BarInstances->ClearInstances();

	const int32 HalfSpectrum = Intensity.Num() / 2;

	for (int32 Index = 0; Index < HalfSpectrum; ++Index)
	{
		const float Height = FMath::Max(static_cast<float>(Intensity[Index]) * VisualizationScale, 10.0f);
		const float XOffset = Index * BarSpacing;
		const FVector Location(XOffset, 0.0f, Height * 0.5f);
		const FVector Scale(BarWidth * 0.01f, BarWidth * 0.01f, Height * 0.01f);
		const FTransform InstanceTransform(FRotator::ZeroRotator, Location, Scale);
		BarInstances->AddInstance(InstanceTransform);
	}
}

void AIntensitySpectrumVisualizerActor::LogIntensitySummary() const
{
	if (Intensity.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Intensity spectrum is empty."));
		return;
	}

	double MaxValue = 0.0;
	double Sum = 0.0;
	for (double Value : Intensity)
	{
		MaxValue = FMath::Max(MaxValue, Value);
		Sum += Value;
	}

	const double Average = Sum / Intensity.Num();
	UE_LOG(LogTemp, Log, TEXT("Intensity summary: Max=%.4f, Avg=%.4f"), MaxValue, Average);
}
