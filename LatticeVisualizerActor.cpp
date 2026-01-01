#include "LatticeVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"

ALatticeVisualizerActor::ALatticeVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	PointInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PointInstances"));
	PointInstances->SetupAttachment(RootComponent);
	PointInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ALatticeVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (!PointMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("PointMesh is not configured; skipping lattice visualization."));
		return;
	}

	PointInstances->SetStaticMesh(PointMesh);
	if (PointMaterial)
	{
		PointInstances->SetMaterial(0, PointMaterial);
	}

	VisualizeLattice();
}

void ALatticeVisualizerActor::VisualizeLattice()
{
	PointInstances->ClearInstances();

	for (int32 X = -Range; X <= Range; ++X)
	{
		for (int32 Y = -Range; Y <= Range; ++Y)
		{
			for (int32 Z = -Range; Z <= Range; ++Z)
			{
				const FVector Location = FVector(X, Y, Z) * Scale;
				const FTransform Transform(FRotator::ZeroRotator, Location, FVector(Scale * 0.01f));
				PointInstances->AddInstance(Transform);
			}
		}
	}

}
