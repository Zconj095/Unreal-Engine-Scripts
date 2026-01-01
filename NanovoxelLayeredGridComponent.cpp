#include "NanovoxelLayeredGridComponent.h"
#include "DrawDebugHelpers.h"

UNanovoxelLayeredGridComponent::UNanovoxelLayeredGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNanovoxelLayeredGridComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GridSize.X <= 0 || GridSize.Y <= 0 || GridSize.Z <= 0 || VoxelSize <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("NanovoxelLayeredGridComponent: Invalid grid configuration."));
		return;
	}

	const int32 Total = GridSize.X * GridSize.Y * GridSize.Z;
	Voxels.SetNumZeroed(Total);

	FVector Origin = GetOwner()->GetActorLocation();
	int32 Index = 0;
	for (int32 X = 0; X < GridSize.X; ++X)
	{
		for (int32 Y = 0; Y < GridSize.Y; ++Y)
		{
			for (int32 Z = 0; Z < GridSize.Z; ++Z)
			{
				const FVector Position = Origin + FVector(X, Y, Z) * VoxelSize;
				Voxels[Index++] = FNanovoxelStruct(Position, FLinearColor::Transparent, 0.0f, false);
			}
		}
	}

	DebugDrawGrid();
}

void UNanovoxelLayeredGridComponent::ActivateLayer(int32 LayerIndex, float Density)
{
	if (LayerIndex < 0 || LayerIndex >= GridSize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivateLayer: Layer %d out of range."), LayerIndex);
		return;
	}

	for (int32 X = 0; X < GridSize.X; ++X)
	{
		for (int32 Z = 0; Z < GridSize.Z; ++Z)
		{
			const int32 Index = GetIndex(FIntVector(X, LayerIndex, Z));
			if (Voxels.IsValidIndex(Index))
			{
				Voxels[Index].bIsActive = true;
				Voxels[Index].Color = LayerColor;
				Voxels[Index].Density = Density;
			}
		}
	}

	DebugDrawGrid();
}

void UNanovoxelLayeredGridComponent::DeactivateLayer(int32 LayerIndex)
{
	if (LayerIndex < 0 || LayerIndex >= GridSize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeactivateLayer: Layer %d out of range."), LayerIndex);
		return;
	}

	for (int32 X = 0; X < GridSize.X; ++X)
	{
		for (int32 Z = 0; Z < GridSize.Z; ++Z)
		{
			const int32 Index = GetIndex(FIntVector(X, LayerIndex, Z));
			if (Voxels.IsValidIndex(Index))
			{
				Voxels[Index].bIsActive = false;
				Voxels[Index].Color = FLinearColor::Transparent;
				Voxels[Index].Density = 0.0f;
			}
		}
	}

	DebugDrawGrid();
}

bool UNanovoxelLayeredGridComponent::IsValidGridPosition(FIntVector Position) const
{
	return Position.X >= 0 && Position.X < GridSize.X &&
		Position.Y >= 0 && Position.Y < GridSize.Y &&
		Position.Z >= 0 && Position.Z < GridSize.Z;
}

int32 UNanovoxelLayeredGridComponent::GetIndex(FIntVector Position) const
{
	return Position.X * GridSize.Y * GridSize.Z + Position.Y * GridSize.Z + Position.Z;
}

void UNanovoxelLayeredGridComponent::DebugDrawGrid() const
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
