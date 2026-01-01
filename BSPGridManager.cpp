#include "BSPGridManager.h"

#include "BSPVoxelGrid.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

ABSPGridManager::ABSPGridManager()
{
    PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITOR
    PrimaryActorTick.bStartWithTickEnabled = true;
#endif
}

void ABSPGridManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if WITH_EDITOR
    DrawGridBounds();
#endif
}

#if WITH_EDITOR
void ABSPGridManager::DrawGridBounds() const
{
    UWorld* World = GetWorld();
    if (!World || Grids.Num() == 0)
    {
        return;
    }

    for (const TObjectPtr<ABSPVoxelGrid>& Grid : Grids)
    {
        ABSPVoxelGrid* GridPtr = Grid.Get();
        if (!IsValid(GridPtr))
        {
            continue;
        }

        const UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GridPtr->GetRootComponent());
        const FVector Center = GridPtr->GetActorLocation();
        const FVector Extent = RootPrimitive ? RootPrimitive->Bounds.BoxExtent : FVector::ZeroVector;
        const FQuat Rotation = GridPtr->GetActorQuat();

        DrawDebugBox(World, Center, Extent, Rotation, FColor::Blue, false, 0.0f, 0, 2.0f);
    }
}
#endif
