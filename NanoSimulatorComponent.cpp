#include "NanoSimulatorComponent.h"
#include "Math/UnrealMathUtility.h"

UNanoSimulatorComponent::UNanoSimulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UNanoSimulatorComponent::SimulateNanoFluctuation(const FVector& Input) const
{
	return Input + FVector(
		FMath::FRandRange(-FluctuationIntensity, FluctuationIntensity),
		FMath::FRandRange(-FluctuationIntensity, FluctuationIntensity),
		FMath::FRandRange(-FluctuationIntensity, FluctuationIntensity));
}
