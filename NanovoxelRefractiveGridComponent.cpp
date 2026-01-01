#include "NanovoxelRefractiveGridComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

UNanovoxelRefractiveGridComponent::UNanovoxelRefractiveGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNanovoxelRefractiveGridComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GridSize.X <= 0 || GridSize.Y <= 0 || GridSize.Z <= 0 || VoxelSize <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("NanovoxelRefractiveGridComponent: Invalid configuration."));
		return;
	}

	const int32 Total = GridSize.X * GridSize.Y * GridSize.Z;
	Voxels.SetNumZeroed(Total);

	FVector Origin = GetOwner()->GetActorLocation();
	int32 Index = 0;
	for (int32 X=0; X<GridSize.X; ++X)
	{
		for (int32 Y=0; Y<GridSize.Y; ++Y)
		{
			for (int32 Z=0; Z<GridSize.Z; ++Z)
			{
				const FVector Position = Origin + FVector(X, Y, Z) * VoxelSize;
				const float RefractiveIndex = FMath::FRandRange(MinRefractiveIndex, MaxRefractiveIndex);
				Voxels[Index++] = FNanovoxelRefractiveStruct(Position, FLinearColor::Transparent, 0.0f, false, RefractiveIndex);
			}
		}
	}

	DebugDrawGrid();
}

void UNanovoxelRefractiveGridComponent::ActivateLayer(int32 LayerIndex, float Density)
{
	if (LayerIndex < 0 || LayerIndex >= GridSize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("Layer %d out of bounds."), LayerIndex);
		return;
	}

	for (int32 X=0; X<GridSize.X; ++X)
	{
		for (int32 Z=0; Z<GridSize.Z; ++Z)
		{
			const int32 Index = GetIndex(FIntVector(X, LayerIndex, Z));
			if (!Voxels.IsValidIndex(Index)) continue;

			FNanovoxelRefractiveStruct& Voxel = Voxels[Index];
			Voxel.bIsActive = true;
			Voxel.Density = Density;
			const float T = (Voxel.RefractiveIndex - MinRefractiveIndex) / FMath::Max(MaxRefractiveIndex - MinRefractiveIndex, KINDA_SMALL_NUMBER);
			Voxel.Color = FLinearColor::LerpUsingHSV(FLinearColor::Blue, FLinearColor::Red, FMath::Clamp(T, 0.0f, 1.0f));
		}
	}

	DebugDrawGrid();
}

void UNanovoxelRefractiveGridComponent::DeactivateLayer(int32 LayerIndex)
{
	if (LayerIndex < 0 || LayerIndex >= GridSize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("Layer %d out of bounds."), LayerIndex);
		return;
	}

	for (int32 X=0; X<GridSize.X; ++X)
	{
		for (int32 Z=0; Z<GridSize.Z; ++Z)
		{
			const int32 Index = GetIndex(FIntVector(X, LayerIndex, Z));
			if (!Voxels.IsValidIndex(Index)) continue;

			FNanovoxelRefractiveStruct& Voxel = Voxels[Index];
			Voxel.bIsActive = false;
			Voxel.Color = FLinearColor::Transparent;
			Voxel.Density = 0.0f;
		}
	}

	DebugDrawGrid();
}

int32 UNanovoxelRefractiveGridComponent::GetIndex(FIntVector GridPosition) const
{
	return GridPosition.X * GridSize.Y * GridSize.Z + GridPosition.Y * GridSize.Z + GridPosition.Z;
}

void UNanovoxelRefractiveGridComponent::DebugDrawGrid() const
{
	if (!GetWorld()) return;

	for (const FNanovoxelRefractiveStruct& Voxel : Voxels)
	{
		if (!Voxel.bIsActive) continue;
		DrawDebugBox(GetWorld(), Voxel.Position, FVector(VoxelSize * 0.5f), Voxel.Color.ToFColor(true), true, -1.0f, 0, 1.0f);
	}
}
