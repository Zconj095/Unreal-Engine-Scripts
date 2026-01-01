#include "MoonlightEnergyConversionActor.h"
#include "Math/UnrealMathUtility.h"

AMoonlightEnergyConversionActor::AMoonlightEnergyConversionActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMoonlightEnergyConversionActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeEnergyFlux();
}

void AMoonlightEnergyConversionActor::ComputeEnergyFlux() const
{
	if (Distance == 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Distance from the moon cannot be zero."));
		return;
	}

	const float Flux = CalculateEnergyFlux(Intensity, Area, Distance);
	UE_LOG(LogTemp, Log, TEXT("Moonlight Energy Flux absorbed: %.6f"), Flux);
}

float AMoonlightEnergyConversionActor::CalculateEnergyFlux(float I, float A, float d) const
{
	return (I * A) / FMath::Pow(d, 2);
}
