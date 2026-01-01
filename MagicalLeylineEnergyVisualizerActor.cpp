#include "MagicalLeylineEnergyVisualizerActor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Components/InstancedStaticMeshComponent.h"

AMagicalLeylineEnergyVisualizerActor::AMagicalLeylineEnergyVisualizerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	NodeInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("NodeInstances"));
	SetRootComponent(NodeInstances);
}

void AMagicalLeylineEnergyVisualizerActor::BeginPlay()
{
	Super::BeginPlay();

	if (NodeMesh)
	{
		NodeInstances->SetStaticMesh(NodeMesh);
	}

	if (NodeMaterial)
	{
		NodeInstances->SetMaterial(0, NodeMaterial);
	}

	const int32 TotalCells = GridSize * GridSize * GridSize;
	TArray<double> Field;
	Field.Reserve(TotalCells);

	for (int32 Z = 0; Z < GridSize; ++Z)
	{
		for (int32 Y = 0; Y < GridSize; ++Y)
		{
			for (int32 X = 0; X < GridSize; ++X)
			{
				Field.Add(GenerateFieldValue(X, Y, Z, GridSpacing));
			}
		}
	}

	VisualizeField(Field);
}

void AMagicalLeylineEnergyVisualizerActor::VisualizeField(const TArray<double>& Field)
{
	if (!NodeInstances || Field.Num() == 0)
	{
		return;
	}

	const float HalfOffset = GridSpacing * 0.5f;
	int32 Index = 0;

	for (int32 Z = 0; Z < GridSize; ++Z)
	{
		for (int32 Y = 0; Y < GridSize; ++Y)
		{
			for (int32 X = 0; X < GridSize; ++X)
			{
				const double Value = Field.IsValidIndex(Index) ? Field[Index] : 0.0;
				const float Magnitude = FMath::Clamp(static_cast<float>(FMath::Abs(Value)) * ScaleMultiplier, 0.1f, 10.f);

				FTransform InstanceTransform;
				const FVector Location = FVector(X * GridSpacing, Y * GridSpacing, Z * GridSpacing);
				InstanceTransform.SetLocation(Location);
				InstanceTransform.SetScale3D(FVector(Magnitude));

				NodeInstances->AddInstance(InstanceTransform);
				++Index;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Visualized %d magical leyline nodes."), Field.Num());
}

double AMagicalLeylineEnergyVisualizerActor::GenerateFieldValue(int32 X, int32 Y, int32 Z, double Dx) const
{
	const double PosX = X * Dx * 0.01;
	const double PosY = Y * Dx * 0.01;
	const double PosZ = Z * Dx * 0.01;

	return FMath::Sin(PosX) * FMath::Cos(PosY) * FMath::Exp(-PosZ);
}
