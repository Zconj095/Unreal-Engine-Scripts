#include "NanovoxelSimulatorComponent.h"
#include "Math/UnrealMathUtility.h"

UNanovoxelSimulatorComponent::UNanovoxelSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNanovoxelSimulatorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNanovoxelSimulatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Grid)
	{
		return;
	}

	if (FMath::RandBool())
	{
		const int32 Density = FMath::RandRange(ActivationDensityMin, ActivationDensityMax);
		Grid->ActivateLayer(GetRandomGridPosition().Y, Density);
	}
}

FIntVector UNanovoxelSimulatorComponent::GetRandomGridPosition() const
{
	if (!Grid)
	{
		return FIntVector::ZeroValue;
	}

	return FIntVector(
		FMath::RandRange(0, Grid->GridSize.X - 1),
		FMath::RandRange(0, Grid->GridSize.Y - 1),
		FMath::RandRange(0, Grid->GridSize.Z - 1));
}
