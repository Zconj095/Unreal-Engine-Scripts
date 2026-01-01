#include "MassEnergyTransformationActor.h"
#include "Math/UnrealMathUtility.h"

AMassEnergyTransformationActor::AMassEnergyTransformationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMassEnergyTransformationActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeEnergy();
}

void AMassEnergyTransformationActor::ComputeEnergy() const
{
	const double Csquared = double(SpeedOfLight) * double(SpeedOfLight);
	const double Energy = double(Mass) * Csquared + double(MagicalCoefficient) * double(MagicalFlux);

	UE_LOG(LogTemp, Log, TEXT("Total Energy (E): %.4f Joules"), Energy);
}
