#include "ElectromagneticParticleComponent.h"

UElectromagneticParticleComponent::UElectromagneticParticleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UElectromagneticParticleComponent::CalculateLorentzForce(const FVector& ElectricField, const FVector& Velocity, const FVector& MagneticField) const
{
	const FVector ElectricForce = Charge * ElectricField;
	const FVector MagneticForce = Charge * FVector::CrossProduct(Velocity, MagneticField);
	return ElectricForce + MagneticForce;
}

