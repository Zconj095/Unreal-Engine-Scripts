#include "PolarizedVoxelGridComponent.h"
#include "DrawDebugHelpers.h"

UPolarizedVoxelGridComponent::UPolarizedVoxelGridComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPolarizedVoxelGridComponent::OnRegister()
{
    Super::OnRegister();
    InitializeGrid();
}

void UPolarizedVoxelGridComponent::InitializeGrid()
{
    const int32 Total = GridDimensions.X * GridDimensions.Y * GridDimensions.Z;
    Voxels.SetNumZeroed(Total);

    for (int32 X = 0; X < GridDimensions.X; ++X)
    {
        for (int32 Y = 0; Y < GridDimensions.Y; ++Y)
        {
            for (int32 Z = 0; Z < GridDimensions.Z; ++Z)
            {
                const int32 Index = GetIndex(X, Y, Z);
                const FVector Position = FVector(X, Y, Z) * VoxelSpacing + OriginOffset + GetOwner()->GetActorLocation();
                Voxels[Index] = FNanoVoxelWithPolarizationStruct(Position, FLinearColor::Transparent, 0.0f, false, 1.0f);
            }
        }
    }
}

bool UPolarizedVoxelGridComponent::GetVoxelData(int32 X, int32 Y, int32 Z, FNanoVoxelWithPolarizationStruct& OutVoxel) const
{
    if (const FNanoVoxelWithPolarizationStruct* Voxel = FindVoxel(X, Y, Z))
    {
        OutVoxel = *Voxel;
        return true;
    }
    return false;
}

void UPolarizedVoxelGridComponent::ActivateLayer(int32 LayerIndex)
{
    if (LayerIndex < 0 || LayerIndex >= GridDimensions.Y) return;

    for (int32 X = 0; X < GridDimensions.X; ++X)
    {
        for (int32 Z = 0; Z < GridDimensions.Z; ++Z)
        {
            if (FNanoVoxelWithPolarizationStruct* Voxel = FindVoxel(X, LayerIndex, Z))
            {
                Voxel->bIsElectricallyActive = true;
                Voxel->DisplayColor = FLinearColor::Red;
            }
        }
    }
}

void UPolarizedVoxelGridComponent::ActivateLayers(int32 Count)
{
    for (int32 Y = 0; Y < Count && Y < GridDimensions.Y; ++Y)
    {
        ActivateLayer(Y);
    }
}

int32 UPolarizedVoxelGridComponent::GetIndex(int32 X, int32 Y, int32 Z) const
{
    return X + GridDimensions.X * (Y + GridDimensions.Y * Z);
}

bool UPolarizedVoxelGridComponent::IsValidCoordinate(int32 X, int32 Y, int32 Z) const
{
    return X >= 0 && X < GridDimensions.X &&
           Y >= 0 && Y < GridDimensions.Y &&
           Z >= 0 && Z < GridDimensions.Z;
}

FNanoVoxelWithPolarizationStruct* UPolarizedVoxelGridComponent::FindVoxel(int32 X, int32 Y, int32 Z)
{
    if (!IsValidCoordinate(X, Y, Z)) return nullptr;
    const int32 Index = GetIndex(X, Y, Z);
    return Voxels.IsValidIndex(Index) ? &Voxels[Index] : nullptr;
}

const FNanoVoxelWithPolarizationStruct* UPolarizedVoxelGridComponent::FindVoxel(int32 X, int32 Y, int32 Z) const
{
    if (!IsValidCoordinate(X, Y, Z)) return nullptr;
    const int32 Index = GetIndex(X, Y, Z);
    return Voxels.IsValidIndex(Index) ? &Voxels[Index] : nullptr;
}
