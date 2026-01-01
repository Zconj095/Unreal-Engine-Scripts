#include "GeoevolutionaryCognitiveExpansionActor.h"

#include "Math/UnrealMathUtility.h"

AGeoevolutionaryCognitiveExpansionActor::AGeoevolutionaryCognitiveExpansionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeoevolutionaryCognitiveExpansionActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGCE();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GCE: %.4f"), GCEValue);
	}
}

void AGeoevolutionaryCognitiveExpansionActor::UpdateTheta(float NewTheta)
{
	Theta = NewTheta;
	RecalculateGCE();
}

void AGeoevolutionaryCognitiveExpansionActor::UpdateTotalNeurons(float NewTotalNeurons)
{
	if (NewTotalNeurons > 0.0f)
	{
		TotalNeurons = NewTotalNeurons;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Total neurons must be greater than zero."));
	}
	RecalculateGCE();
}

void AGeoevolutionaryCognitiveExpansionActor::UpdateDimensionality(float NewDimensionality)
{
	Dimensionality = NewDimensionality;
	RecalculateGCE();
}

void AGeoevolutionaryCognitiveExpansionActor::UpdateConnectivityDensity(float NewConnectivityDensity)
{
	if (NewConnectivityDensity >= 0.0f)
	{
		ConnectivityDensity = NewConnectivityDensity;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Connectivity density must be non-negative."));
	}
	RecalculateGCE();
}

float AGeoevolutionaryCognitiveExpansionActor::GetGCE() const
{
	return GCEValue;
}

void AGeoevolutionaryCognitiveExpansionActor::RecalculateGCE()
{
	if (TotalNeurons <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Total neurons (N) must be greater than zero."));
		GCEValue = 0.0f;
		return;
	}

	GCEValue = Theta * FMath::Pow(TotalNeurons, Dimensionality) * FMath::Loge(1.0f + ConnectivityDensity) / FMath::Loge(2.0f);
}
