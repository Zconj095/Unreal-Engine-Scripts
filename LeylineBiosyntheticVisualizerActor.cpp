#include "LeylineBiosyntheticVisualizerActor.h"
#include "Engine/Engine.h"
#include "Engine/CollisionProfile.h"

ALeylineBiosyntheticVisualizerActor::ALeylineBiosyntheticVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	NodeInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("NodeInstances"));
	NodeInstances->SetupAttachment(RootComponent);
	NodeInstances->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ALeylineBiosyntheticVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (!NodeMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("NodeMesh not configured; skipping visualization."));
		return;
	}

	NodeInstances->SetStaticMesh(NodeMesh);
	if (NodeMaterial)
	{
		NodeInstances->SetMaterial(0, NodeMaterial);
	}

	TArray<double> Field = ULeylineBiosynthesisUtility::GenerateMagicalField(GridSize, Dx);
	TArray<FVector> Gradient;
	ULeylineBiosynthesisUtility::ComputeGradient(GridSize, Dx, Field, Gradient);

	const double TotalEnergy = ULeylineBiosynthesisUtility::ComputeBiosyntheticEnergy(GridSize, Dx, Field, Gradient);
	UE_LOG(LogTemp, Log, TEXT("Total Biosynthetic Energy: %.4f"), TotalEnergy);

	VisualizeField(Field);
}

void ALeylineBiosyntheticVisualizerActor::VisualizeField(const TArray<double>& Field)
{
	NodeInstances->ClearInstances();

	for (int32 X = 0; X < GridSize; ++X)
	{
		for (int32 Y = 0; Y < GridSize; ++Y)
		{
			for (int32 Z = 0; Z < GridSize; ++Z)
			{
				const int32 Index = X + GridSize * (Y + GridSize * Z);
				const double Value = Field.IsValidIndex(Index) ? Field[Index] : 0.0;
				const float Scale = FMath::Max(static_cast<float>(Value) * HeightScale, 0.1f);
				const FVector Location = FVector(X * PointScale, Y * PointScale, Z * PointScale);
				const FVector Scale3D = FVector(Scale);
				const FTransform Transform(FRotator::ZeroRotator, Location, Scale3D);
				NodeInstances->AddInstance(Transform);
			}
		}
	}
}
