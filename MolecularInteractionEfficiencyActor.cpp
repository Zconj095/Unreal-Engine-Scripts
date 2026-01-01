#include "MolecularInteractionEfficiencyActor.h"
#include "Math/UnrealMathUtility.h"

AMolecularInteractionEfficiencyActor::AMolecularInteractionEfficiencyActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMolecularInteractionEfficiencyActor::BeginPlay()
{
	Super::BeginPlay();

	CalculateEfficiency();
}

void AMolecularInteractionEfficiencyActor::CalculateEfficiency() const
{
	if (EnergyUsed == 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Energy used (ΔE) is zero. Cannot compute efficiency."));
		return;
	}

	const float Efficiency = CognitiveWork / EnergyUsed;
	UE_LOG(LogTemp, Log, TEXT("Molecular Interaction Efficiency (η_m): %.4f"), Efficiency);
}
