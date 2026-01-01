#include "LeylineEnergyVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"
#include "Math/UnrealMathUtility.h"

ALeylineEnergyVisualizerActor::ALeylineEnergyVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	PointInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PointInstances"));
	PointInstances->SetupAttachment(RootComponent);
	PointInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ALeylineEnergyVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (!PointMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("PointMesh not configured; skipping visualization."));
		return;
	}

	PointInstances->SetStaticMesh(PointMesh);
	if (PointMaterial)
	{
		PointInstances->SetMaterial(0, PointMaterial);
	}

	TArray<double> Field;
	for (int32 I = 0; I < GridSize; ++I)
	{
		for (int32 J = 0; J < GridSize; ++J)
		{
			for (int32 K = 0; K < GridSize; ++K)
			{
				const double Value = FMath::Sin(static_cast<double>(I));
				Field.Add(Value);
			}
		}
	}

	VisualizeField(Field);
}

void ALeylineEnergyVisualizerActor::VisualizeField(const TArray<double>& Field)
{
	PointInstances->ClearInstances();

	for (int32 Index = 0; Index < Field.Num(); ++Index)
	{
		if (Field[Index] <= DensityThreshold)
		{
			continue;
		}

		const int32 X = Index % GridSize;
		const int32 Y = (Index / GridSize) % GridSize;
		const int32 Z = Index / (GridSize * GridSize);

		const FVector Location(X * 100.0f, Y * 100.0f, Z * 100.0f);
		const FTransform Transform(FRotator::ZeroRotator, Location, FVector(0.5f));
		PointInstances->AddInstance(Transform);
	}
}
