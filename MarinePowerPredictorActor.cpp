#include "MarinePowerPredictorActor.h"
#include "Math/UnrealMathUtility.h"

AMarinePowerPredictorActor::AMarinePowerPredictorActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMarinePowerPredictorActor::BeginPlay()
{
	Super::BeginPlay();

	LogPrediction();
}

void AMarinePowerPredictorActor::LogPrediction() const
{
	float Sum = 0.f;
	for (float Value : FieldStrengths)
	{
		Sum += Value;
	}

	for (float Value : EnergyGradient)
	{
		Sum += Value;
	}

	const float Prediction = Sum / (FieldStrengths.Num() + EnergyGradient.Num());
	UE_LOG(LogTemp, Log, TEXT("Predicted Marine Power: %f"), Prediction);
}
