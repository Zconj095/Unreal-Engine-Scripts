#include "BSPVoxelGrid.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

ABSPVoxelGrid::ABSPVoxelGrid()
{
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif
}

void ABSPVoxelGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InitializeBSP();
}

void ABSPVoxelGrid::BeginPlay()
{
	Super::BeginPlay();

	InitializeBSP();
	UE_LOG(LogTemp, Log, TEXT("BSP tree initialized."));
}

void ABSPVoxelGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if (bEnableGizmos && RootNode && GetWorld())
	{
		DrawLeafBounds(RootNode.Get());
	}
#endif
}

void ABSPVoxelGrid::InitializeBSP()
{
	if (GridSize.X <= 0 || GridSize.Y <= 0 || GridSize.Z <= 0)
	{
		RootNode.Reset();
		return;
	}

	const int32 TotalVoxels = GridSize.X * GridSize.Y * GridSize.Z;
	if (TotalVoxels <= 0)
	{
		RootNode.Reset();
		return;
	}

	TArray<FNanovoxel> Voxels;
	Voxels.Reserve(TotalVoxels);
	BuildNanovoxels(Voxels);

	const FBox GridBounds = CalculateGridBounds();
	RootNode = MakeUnique<FBSPNode>(GridBounds, MoveTemp(Voxels));
	RootNode->Subdivide();
}

void ABSPVoxelGrid::BuildNanovoxels(TArray<FNanovoxel>& OutVoxels) const
{
	const FVector Origin = GetActorLocation();

	for (int32 X = 0; X < GridSize.X; ++X)
	{
		for (int32 Y = 0; Y < GridSize.Y; ++Y)
		{
			for (int32 Z = 0; Z < GridSize.Z; ++Z)
			{
				FNanovoxel Voxel;
				Voxel.Position = Origin + FVector(X, Y, Z) * VoxelSize;
				Voxel.Color = FLinearColor::Gray;
				Voxel.Density = 1.0f;
				Voxel.bActive = FMath::FRand() > 0.5f;

				OutVoxels.Add(Voxel);
			}
		}
	}
}

FBox ABSPVoxelGrid::CalculateGridBounds() const
{
	const FVector GridWorldSize(GridSize.X, GridSize.Y, GridSize.Z);
	const FVector HalfWorldSize = GridWorldSize * VoxelSize * 0.5f;
	const FVector Center = GetActorLocation() + HalfWorldSize;
	return FBox(Center - HalfWorldSize, Center + HalfWorldSize);
}

#if WITH_EDITOR
void ABSPVoxelGrid::DrawLeafBounds(const FBSPNode* Node) const
{
	if (!Node || !GetWorld())
	{
		return;
	}

	if (Node->IsLeaf())
	{
		const FBox& Bounds = Node->GetBounds();
		DrawDebugBox(GetWorld(), Bounds.GetCenter(), Bounds.GetExtent(), FColor::Yellow, false, 0.0f, 0, 1.0f);
		return;
	}

	DrawLeafBounds(Node->GetLeftChild());
	DrawLeafBounds(Node->GetRightChild());
}
#endif
