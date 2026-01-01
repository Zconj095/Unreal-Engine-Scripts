#include "MarinePowerVisualizerActor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

AMarinePowerVisualizerActor::AMarinePowerVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RegionInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RegionInstances"));
	SetRootComponent(RegionInstances);

	MarineInfluenceComponent = CreateDefaultSubobject<ULeylineMarineInfluenceComponent>(TEXT("LeylineMarineInfluence"));
}

void AMarinePowerVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (RegionMesh)
	{
		RegionInstances->SetStaticMesh(RegionMesh);
	}

	if (RegionMaterial)
	{
		RegionInstances->SetMaterial(0, RegionMaterial);
	}

	if (MarineInfluenceComponent)
	{
		MarineInfluenceComponent->RunTest();
	}

	SpawnRegions();
}

void AMarinePowerVisualizerActor::SpawnRegions()
{
	const TArray<FVector> Positions = {
		FVector(0.f, 0.f, 0.f),
		FVector(200.f, 0.f, 0.f)
	};

	for (const FVector& Position : Positions)
	{
		FTransform Transform;
		Transform.SetLocation(Position);
		Transform.SetScale3D(FVector(1.5f));
		RegionInstances->AddInstance(Transform);
	}

	UE_LOG(LogTemp, Log, TEXT("Spawned %d marine regions for visualization."), Positions.Num());
}
