#include "MutationRateVisualizerActor.h"
#include "Math/UnrealMathUtility.h"

AMutationRateVisualizerActor::AMutationRateVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PointInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PointInstances"));
	SetRootComponent(PointInstances);
}

void AMutationRateVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (PointMesh)
	{
		PointInstances->SetStaticMesh(PointMesh);
	}
	if (PointMaterial)
	{
		PointInstances->SetMaterial(0, PointMaterial);
	}

	InitializeTimeline();
	VisualizeRates();
}

void AMutationRateVisualizerActor::InitializeTimeline()
{
	if (MagicalEnergyTimeline.Num() == 0)
	{
		MagicalEnergyTimeline = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f};
	}
}

void AMutationRateVisualizerActor::VisualizeRates()
{
	for (int32 Index = 0; Index < MagicalEnergyTimeline.Num(); ++Index)
	{
		const float Energy = MagicalEnergyTimeline[Index];
		const float Height = ComputeMutationRate(Energy);
		FTransform Transform;
		Transform.SetLocation(FVector(Index * 100.f, Height * 100.f, 0.f));
		Transform.SetScale3D(FVector(0.5f, Height * 0.5f, 0.5f));
		PointInstances->AddInstance(Transform);
	}

	UE_LOG(LogTemp, Log, TEXT("Visualized %d mutation rate points."), MagicalEnergyTimeline.Num());
}

float AMutationRateVisualizerActor::ComputeMutationRate(float MagicalEnergy) const
{
	return BaselineRate + MagicEnhancement * MagicalEnergy;
}
