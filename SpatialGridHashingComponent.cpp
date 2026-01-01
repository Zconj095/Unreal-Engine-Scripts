#include "SpatialGridHashingComponent.h"

USpatialGridHashingComponent::USpatialGridHashingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USpatialGridHashingComponent::Initialize(float InCellSize)
{
    CellSize = FMath::Max(InCellSize, 0.01f);
    ClearGrid();
}

void USpatialGridHashingComponent::ClearGrid()
{
    Grid.Reset();
}

void USpatialGridHashingComponent::AddParticle(const FVector& Position, int32 ParticleIndex)
{
    const int32 CellID = GetCellID(Position);
    TArray<int32>& Cell = Grid.FindOrAdd(CellID);
    Cell.AddUnique(ParticleIndex);
}

TArray<int32> USpatialGridHashingComponent::GetNeighbors(const FVector& Position) const
{
    const int32 CellID = GetCellID(Position);
    if (const TArray<int32>* Cell = Grid.Find(CellID))
    {
        return *Cell;
    }
    return {};
}

int32 USpatialGridHashingComponent::GetCellID(const FVector& Position) const
{
    const int32 X = FMath::FloorToInt(Position.X / CellSize);
    const int32 Y = FMath::FloorToInt(Position.Y / CellSize);
    const int32 Z = FMath::FloorToInt(Position.Z / CellSize);
    return X + Y * 73856093 + Z * 19349663;
}
