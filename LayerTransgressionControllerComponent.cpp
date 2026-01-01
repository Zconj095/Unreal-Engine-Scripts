#include "LayerTransgressionControllerComponent.h"
#include "Engine/World.h"

ULayerTransgressionControllerComponent::ULayerTransgressionControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULayerTransgressionControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!VoxelGrid || !IsValid(VoxelGrid))
	{
		UE_LOG(LogTemp, Warning, TEXT("LayerTransgressionControllerComponent requires a valid VoxelGrid."));
		SetComponentTickEnabled(false);
	}
}

void ULayerTransgressionControllerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!VoxelGrid || VoxelGrid->GridSize.Y <= 0)
	{
		return;
	}

	Timer += DeltaTime;
	if (Timer < ActivationInterval)
	{
		return;
	}

	Timer = 0.0f;
	ActivateNextLayer();
}

void ULayerTransgressionControllerComponent::ActivateNextLayer()
{
	if (!VoxelGrid)
	{
		return;
	}

	if (CurrentLayer >= VoxelGrid->GridSize.Y)
	{
		UE_LOG(LogTemp, Log, TEXT("LayerTransgressionControllerComponent: All layers activated."));
		SetComponentTickEnabled(false);
		return;
	}

	VoxelGrid->ActivateLayer(CurrentLayer, LayerDensity);
	CurrentLayer++;
}
