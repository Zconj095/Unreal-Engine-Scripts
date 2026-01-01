#include "GraphicsEngineOptimizationIndexActor.h"

#include "Math/UnrealMathUtility.h"

AGraphicsEngineOptimizationIndexActor::AGraphicsEngineOptimizationIndexActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGraphicsEngineOptimizationIndexActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGEOI();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GEOI: %.4f"), GEOIValue);
	}
}

void AGraphicsEngineOptimizationIndexActor::UpdateZeta(float NewZeta)
{
	Zeta = NewZeta;
	RecalculateGEOI();
}

void AGraphicsEngineOptimizationIndexActor::UpdatePhi(float NewPhi)
{
	Phi = NewPhi;
	RecalculateGEOI();
}

void AGraphicsEngineOptimizationIndexActor::UpdateMetabolicCost(float NewMetabolicCost)
{
	if (NewMetabolicCost > KINDA_SMALL_NUMBER)
	{
		MetabolicCost = NewMetabolicCost;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Metabolic cost must be greater than zero."));
	}
	RecalculateGEOI();
}

void AGraphicsEngineOptimizationIndexActor::UpdateSynapticDensity(float NewSynapticDensity)
{
	if (NewSynapticDensity > KINDA_SMALL_NUMBER)
	{
		SynapticDensity = NewSynapticDensity;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Synaptic density must be greater than zero."));
	}
	RecalculateGEOI();
}

float AGraphicsEngineOptimizationIndexActor::GetGEOI() const
{
	return GEOIValue;
}

void AGraphicsEngineOptimizationIndexActor::RecalculateGEOI()
{
	if (MetabolicCost <= KINDA_SMALL_NUMBER || SynapticDensity <= KINDA_SMALL_NUMBER)
	{
		GEOIValue = 0.0f;
		return;
	}
	GEOIValue = Zeta * (Phi / (MetabolicCost * SynapticDensity));
}
