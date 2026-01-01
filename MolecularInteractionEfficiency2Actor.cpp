#include "MolecularInteractionEfficiency2Actor.h"
#include "Math/UnrealMathUtility.h"

AMolecularInteractionEfficiency2Actor::AMolecularInteractionEfficiency2Actor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMolecularInteractionEfficiency2Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	const float Efficiency = CalculateEfficiency();
	UE_LOG(LogTemp, Log, TEXT("[%.2f] ηm: %.4f"), ElapsedTime, Efficiency);
}

float AMolecularInteractionEfficiency2Actor::CalculateEfficiency() const
{
	if (EnergyUtilized == 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Energy utilized (ΔE) cannot be zero. Returning efficiency as 0."));
		return 0.f;
	}

	return CognitiveWork / EnergyUtilized;
}
