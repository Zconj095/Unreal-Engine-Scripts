#include "GeologicalDivergenceRatioActor.h"

#include "Math/UnrealMathUtility.h"

AGeologicalDivergenceRatioActor::AGeologicalDivergenceRatioActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGeologicalDivergenceRatioActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RecalculateGDR();
	if (bLogEachTick)
	{
		UE_LOG(LogTemp, Display, TEXT("GDR: %.4f"), GDRValue);
	}
}

void AGeologicalDivergenceRatioActor::UpdateDeltaR(float NewDeltaR)
{
	DeltaR = NewDeltaR;
	RecalculateGDR();
}

void AGeologicalDivergenceRatioActor::UpdateOmega(float NewOmega)
{
	if (NewOmega > KINDA_SMALL_NUMBER)
	{
		Omega = NewOmega;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Omega must be greater than zero."));
	}
	RecalculateGDR();
}

void AGeologicalDivergenceRatioActor::UpdateLambda(float NewLambda)
{
	Lambda = NewLambda;
	RecalculateGDR();
}

float AGeologicalDivergenceRatioActor::GetGDR() const
{
	return GDRValue;
}

void AGeologicalDivergenceRatioActor::RecalculateGDR()
{
	GDRValue = CalculateGDR();
}

float AGeologicalDivergenceRatioActor::CalculateGDR() const
{
	if (Omega <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Error, TEXT("Omega must be greater than zero."));
		return 0.0f;
	}
	return (DeltaR / Omega) * Lambda;
}
