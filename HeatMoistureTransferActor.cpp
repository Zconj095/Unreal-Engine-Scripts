#include "HeatMoistureTransferActor.h"

#include "Math/UnrealMathUtility.h"
#include "Logging/LogMacros.h"

AHeatMoistureTransferActor::AHeatMoistureTransferActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHeatMoistureTransferActor::BeginPlay()
{
	Super::BeginPlay();

	HeatEnergy = CalculateHeatEnergy(WaterMass, LatentHeat);
	if (bLogOnStart)
	{
		UE_LOG(LogTemp, Display, TEXT("Heat Energy (Q): %.2f kJ"), HeatEnergy);
	}

	TArray<double> Prediction = PredictHeatEnergy(WaterMass, LatentHeat);
	UE_LOG(LogTemp, Display, TEXT("Predicted Heat Energy (Q): %.2f kJ"), Prediction.Num() > 0 ? Prediction[0] : 0.0);
	LogTensorFlowApproximation(WaterMass, LatentHeat);
}

double AHeatMoistureTransferActor::CalculateHeatEnergy(double WaterMassValue, double LatentHeatValue) const
{
	return WaterMassValue * LatentHeatValue;
}

TArray<double> AHeatMoistureTransferActor::PredictHeatEnergy(double WaterMassValue, double LatentHeatValue) const
{
	double Magnitude = CalculateHeatEnergy(WaterMassValue, LatentHeatValue);
	TArray<double> Result;
	Result.Add(Magnitude);
	return Result;
}

void AHeatMoistureTransferActor::LogTensorFlowApproximation(double WaterMassValue, double LatentHeatValue) const
{
	double Outcome = CalculateHeatEnergy(WaterMassValue, LatentHeatValue);
	UE_LOG(LogTemp, Display, TEXT("TensorFlow Computation Result (Q): %.2f kJ"), Outcome);
}
