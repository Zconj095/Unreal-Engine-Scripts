#include "PlasmaDynamicsComponent.h"
#include "Logging/LogMacros.h"

UPlasmaDynamicsComponent::UPlasmaDynamicsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlasmaDynamicsComponent::BeginPlay()
{
	Super::BeginPlay();

	const FVector PressureGradient = -Pressure / Density * FVector::OneVector;
	const FVector LorentzForce = FVector::CrossProduct(CurrentDensity, MagneticField);
	const FVector ViscosityForce = Viscosity * FVector::ZeroVector; // Simplified: assume negligible Laplacian

	const FVector Acceleration = PressureGradient + LorentzForce + ViscosityForce;
	UpdatedVelocity = Velocity + Acceleration * TimeStep;

	UE_LOG(LogTemp, Log, TEXT("Updated Plasma Velocity: %s"), *UpdatedVelocity.ToString());
}
