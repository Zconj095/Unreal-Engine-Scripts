#include "PhysicsSimulationComponent.h"
#include "Logging/LogMacros.h"

UPhysicsSimulationComponent::UPhysicsSimulationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPhysicsSimulationComponent::BeginPlay()
{
	Super::BeginPlay();

	NewtonianForce = AccelerationVector * Mass;
	MagicalForce = MagicGradient + FVector(MagicFieldStrength, MagicFieldStrength, MagicFieldStrength);
	CombinedForce = NewtonianForce.Size() + MagicalForce.Size();

	UE_LOG(LogTemp, Log, TEXT("Force (F) using Newtonian Motion: %s"), *NewtonianForce.ToString());
	UE_LOG(LogTemp, Log, TEXT("Magical Force (F_magical): %s"), *MagicalForce.ToString());
	UE_LOG(LogTemp, Log, TEXT("Predicted Force (F and F_magical): %0.4f"), CombinedForce);
}
