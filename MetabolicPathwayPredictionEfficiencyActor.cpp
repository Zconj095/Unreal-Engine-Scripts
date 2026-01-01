#include "MetabolicPathwayPredictionEfficiencyActor.h"
#include "Math/UnrealMathUtility.h"

AMetabolicPathwayPredictionEfficiencyActor::AMetabolicPathwayPredictionEfficiencyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMetabolicPathwayPredictionEfficiencyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Elapsed += DeltaTime;
	if (PredictionTime == 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("Prediction time must not be zero."));
		return;
	}

	MPPEValue = CalculateMPPE();
	UE_LOG(LogTemp, Log, TEXT("[%.2f] MPPE Result: %.4f"), Elapsed, MPPEValue);
}

float AMetabolicPathwayPredictionEfficiencyActor::CalculateMPPE() const
{
	return Eta * (DeltaMpred / PredictionTime);
}
