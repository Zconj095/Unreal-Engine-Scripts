#include "EnzymaticReactionModelComponent.h"
#include "Math/UnrealMathUtility.h"

UEnzymaticReactionModelComponent::UEnzymaticReactionModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Vmax = 10.0;
	Km = 5.0;
	Lambda = 2.0;
	SubstrateConcentrations = {1.0, 2.0, 3.0, 4.0, 5.0};
	MagicalEnergies = {0.5, 1.0, 1.5, 2.0, 2.5};
}

void UEnzymaticReactionModelComponent::BeginPlay()
{
	Super::BeginPlay();
	SimulateVelocities();
}

double UEnzymaticReactionModelComponent::ComputeReactionVelocity(double SubstrateConcentration, double MagicalEnergy) const
{
	const double MichaelisMenten = (Vmax * SubstrateConcentration) / (Km + SubstrateConcentration);
	const double MagicalEffect = Lambda * MagicalEnergy;
	return MichaelisMenten + MagicalEffect;
}

void UEnzymaticReactionModelComponent::SimulateVelocities() const
{
	if (SubstrateConcentrations.Num() != MagicalEnergies.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Substrate and magical arrays must match lengths."));
		return;
	}

	for (int32 Index = 0; Index < SubstrateConcentrations.Num(); ++Index)
	{
		const double Velocity = ComputeReactionVelocity(SubstrateConcentrations[Index], MagicalEnergies[Index]);
		UE_LOG(LogTemp, Log, TEXT("Time %d: v = %.4f"), Index, Velocity);
	}
}
