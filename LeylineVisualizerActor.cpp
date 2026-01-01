#include "LeylineVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"

ALeylineVisualizerActor::ALeylineVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	PointInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PointInstances"));
	PointInstances->SetupAttachment(RootComponent);
	PointInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ALeylineVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector BoundsMin(-10.0f);
	const FVector BoundsMax(10.0f);
	const double EnergyFlow = ULeylineEnergyFlowUtility::ComputeLeylineEnergyFlow(BoundsMin, BoundsMax, 10);
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Flow: %.2f"), EnergyFlow);

	if (!PointMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("PointMesh not set; skipping grid visualization."));
		return;
	}

	PointInstances->SetStaticMesh(PointMesh);
	if (PointMaterial)
	{
		PointInstances->SetMaterial(0, PointMaterial);
	}

	SpawnEnergyPoints();
}

void ALeylineVisualizerActor::SpawnEnergyPoints()
{
	PointInstances->ClearInstances();

	for (int32 X = -GridExtent; X <= GridExtent; ++X)
	{
		for (int32 Y = -GridExtent; Y <= GridExtent; ++Y)
		{
			for (int32 Z = -GridExtent; Z <= GridExtent; ++Z)
			{
				const FVector Location = FVector(X * Spacing, Y * Spacing, Z * Spacing) + GetActorLocation();
				const FTransform Transform(FRotator::ZeroRotator, Location, FVector(0.2f));
				PointInstances->AddInstance(Transform);
			}
		}
	}
}
