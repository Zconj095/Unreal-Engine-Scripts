#include "LuminicTapestryActor.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

ALuminicTapestryActor::ALuminicTapestryActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALuminicTapestryActor::BeginPlay()
{
	Super::BeginPlay();
	CreateThreads();
}

void ALuminicTapestryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateThreads(GetWorld()->GetTimeSeconds());
}

void ALuminicTapestryActor::CreateThreads()
{
	for (int32 Index = 0; Index < ThreadCount; ++Index)
	{
		USplineComponent* Spline = NewObject<USplineComponent>(this);
		Spline->RegisterComponent();
		Spline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		const TArray<FVector> Points = GenerateThreadPoints(Index);
		for (const FVector& Location : Points)
		{
			Spline->AddSplinePoint(Location, ESplineCoordinateSpace::Local);
		}

		Spline->SetSplinePointType(Spline->GetNumberOfSplinePoints() - 1, ESplinePointType::CurveClamped);
		Threads.Add(Spline);
	}
}

TArray<FVector> ALuminicTapestryActor::GenerateThreadPoints(int32 ThreadIndex) const
{
	TArray<FVector> Points;
	const float Offset = ThreadIndex * 0.5f;

	for (int32 PointIndex = 0; PointIndex < PointsPerThread; ++PointIndex)
	{
		const float T = PointIndex / static_cast<float>(PointsPerThread - 1);
		const float X = T * ThreadLength;
		const float Y = Amplitude * FMath::Sin(2.0f * PI * Frequency * T + Offset);
		const float Z = Amplitude * FMath::Cos(2.0f * PI * Frequency * T + Offset);
		Points.Add(FVector(X, Y, Z));
	}

	return Points;
}

void ALuminicTapestryActor::UpdateThreads(float Time) const
{
	for (int32 Index = 0; Index < Threads.Num(); ++Index)
	{
		USplineComponent* Spline = Threads[Index];
		if (!Spline)
		{
			continue;
		}

		const TArray<FVector> Points = GenerateThreadPoints(Index);
		for (int32 PointIndex = 0; PointIndex < Points.Num(); ++PointIndex)
		{
			FVector Modified = Points[PointIndex];
			Modified.Y += FMath::Sin(Time * Frequency + PointIndex * 0.1f) * 10.0f;
			Spline->SetLocationAtSplinePoint(PointIndex, Modified, ESplineCoordinateSpace::Local);
		}
	}
}
