#include "GeochemicalNetworkAdaptabilityActor.h"

AGeochemicalNetworkAdaptabilityActor::AGeochemicalNetworkAdaptabilityActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeochemicalNetworkAdaptabilityActor::BeginPlay()
{
	Super::BeginPlay();

	GNA = CalculateGNA(Gamma, DeltaNGeo, DeltaTime);
	if (bLogResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Geochemical Network Adaptability (GNA): %.4f"), GNA);
	}
}

float AGeochemicalNetworkAdaptabilityActor::CalculateGNA(float InGamma, float InDeltaNGeo, float InDeltaTime) const
{
	if (FMath::IsNearlyZero(InDeltaTime))
	{
		UE_LOG(LogTemp, Error, TEXT("Time interval (Δt) must not be zero."));
		return 0.0f;
	}

	return InGamma * (InDeltaNGeo / InDeltaTime);
}

float AGeochemicalNetworkAdaptabilityActor::GetGNA() const
{
	return GNA;
}
