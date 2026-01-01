#include "GeoevolutionaryProjectionIndexActor.h"

AGeoevolutionaryProjectionIndexActor::AGeoevolutionaryProjectionIndexActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeoevolutionaryProjectionIndexActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGPI();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GPI: %.4f"), GPIValue);
	}
}

void AGeoevolutionaryProjectionIndexActor::UpdateGamma(float NewGamma)
{
	Gamma = NewGamma;
	RecalculateGPI();
}

void AGeoevolutionaryProjectionIndexActor::UpdatePGeo(float NewPGeo)
{
	PGeo = NewPGeo;
	RecalculateGPI();
}

void AGeoevolutionaryProjectionIndexActor::UpdateTimeTaken(float NewTimeTaken)
{
	if (NewTimeTaken > KINDA_SMALL_NUMBER)
	{
		TimeTaken = NewTimeTaken;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Time for cognitive processing must be greater than zero."));
	}
	RecalculateGPI();
}

void AGeoevolutionaryProjectionIndexActor::UpdateMetabolicCost(float NewMetabolicCost)
{
	if (NewMetabolicCost > KINDA_SMALL_NUMBER)
	{
		MetabolicCost = NewMetabolicCost;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Metabolic cost must be greater than zero."));
	}
	RecalculateGPI();
}

float AGeoevolutionaryProjectionIndexActor::GetGPI() const
{
	return GPIValue;
}

float AGeoevolutionaryProjectionIndexActor::CalculateGPI(float InGamma, float InPGeo, float InTimeTaken, float InMetabolicCost) const
{
	if (InTimeTaken <= KINDA_SMALL_NUMBER || InMetabolicCost <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Time and metabolic cost must be greater than zero."));
		return 0.0f;
	}

	return InGamma * (InPGeo / (InTimeTaken * InMetabolicCost));
}

void AGeoevolutionaryProjectionIndexActor::RecalculateGPI()
{
	GPIValue = CalculateGPI(Gamma, PGeo, TimeTaken, MetabolicCost);
}
