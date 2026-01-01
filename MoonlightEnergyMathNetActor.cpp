#include "MoonlightEnergyMathNetActor.h"

AMoonlightEnergyMathNetActor::AMoonlightEnergyMathNetActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMoonlightEnergyMathNetActor::BeginPlay()
{
	Super::BeginPlay();

	ComputeFlux();
}

void AMoonlightEnergyMathNetActor::ComputeFlux() const
{
	if (Distance == 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Distance cannot be zero."));
		return;
	}

	const float Flux = (Intensity * Area) / FMath::Pow(Distance, 2);
	UE_LOG(LogTemp, Log, TEXT("Moonlight Energy Flux via MathNet: %.4f units"), Flux);
}
