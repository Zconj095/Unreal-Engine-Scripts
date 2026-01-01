#include "NanovoxelGridComponent.h"
#include "DrawDebugHelpers.h"

UNanovoxelGridComponent::UNanovoxelGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNanovoxelGridComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GridSize.X <= 0 || GridSize.Y <= 0 || GridSize.Z <= 0 || VoxelSize <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("NanovoxelGridComponent: Invalid grid settings."));
		return;
	}

	const int32 TotalVoxels = GridSize.X * GridSize.Y * GridSize.Z;
	Voxels.SetNumZeroed(TotalVoxels);

	FVector BaseLocation = GetOwner()->GetActorLocation();
	int32 Index = 0;
	for (int32 X = 0; X < GridSize.X; ++X)
	{
		for (int32 Y = 0; Y < GridSize.Y; ++Y)
		{
			for (int32 Z = 0; Z < GridSize.Z; ++Z)
			{
				const FVector Position = BaseLocation + FVector(X, Y, Z) * VoxelSize;
				Voxels[Index++] = FNanovoxelStruct(Position, DefaultColor, 1.0f, false);
			}
		}
	}

	DebugDrawGrid();
}

void UNanovoxelGridComponent::ActivateVoxel(FIntVector GridPosition, FLinearColor Color, float Density)
{
	if (!IsValidGridPosition(GridPosition))
	{
		return;
	}

	const int32 Index = GetIndex(GridPosition);
	if (!Voxels.IsValidIndex(Index))
	{
		return;
	}

	FNanovoxelStruct& Voxel = Voxels[Index];
	Voxel.bIsActive = true;
	Voxel.Color = Color;
	Voxel.Density = Density;
}

bool UNanovoxelGridComponent::IsValidGridPosition(FIntVector GridPosition) const
{
	return GridPosition.X >= 0 && GridPosition.X < GridSize.X &&
		GridPosition.Y >= 0 && GridPosition.Y < GridSize.Y &&
		GridPosition.Z >= 0 && GridPosition.Z < GridSize.Z;
}

int32 UNanovoxelGridComponent::GetIndex(FIntVector GridPosition) const
{
	return GridPosition.X * GridSize.Y * GridSize.Z + GridPosition.Y * GridSize.Z + GridPosition.Z;
}

void UNanovoxelGridComponent::DebugDrawGrid() const
{
	if (!GetWorld())
	{
		return;
	}

	for (const FNanovoxelStruct& Voxel : Voxels)
	{
		if (!Voxel.bIsActive)
		{
			continue;
		}

		DrawDebugBox(GetWorld(), Voxel.Position, FVector(VoxelSize * 0.5f), Voxel.Color.ToFColor(true), true, -1.0f, 0, 1.0f);
	}
}
