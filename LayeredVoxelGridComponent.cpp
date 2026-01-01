#include "LayeredVoxelGridComponent.h"
#include "DrawDebugHelpers.h"

ULayeredVoxelGridComponent::ULayeredVoxelGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULayeredVoxelGridComponent::ActivateLayer(int32 LayerIndex, float Density)
{
	if (LayerIndex < 0 || LayerIndex >= GridSize.Y)
	{
		UE_LOG(LogTemp, Warning, TEXT("LayeredVoxelGridComponent::ActivateLayer - LayerIndex %d out of range."), LayerIndex);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Activating layer %d with density %.3f on %s"), LayerIndex, Density, *GetOwner()->GetName());
}
