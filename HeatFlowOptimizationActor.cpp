#include "HeatFlowOptimizationActor.h"

#include "Math/UnrealMathUtility.h"
#include "Logging/LogMacros.h"

AHeatFlowOptimizationActor::AHeatFlowOptimizationActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHeatFlowOptimizationActor::BeginPlay()
{
	Super::BeginPlay();
	HeatFlux = CalculateHeatFlux(ThermalConductivity, TemperatureGradient, MagicalEnergyFlux);
	if (bLogOnStart)
	{
		FString FluxString;
		for (float Value : HeatFlux)
		{
			FluxString += FString::Printf(TEXT("%.2f "), Value);
		}
		UE_LOG(LogTemp, Display, TEXT("Heat Flux (q): %s"), *FluxString);
	}
}

TArray<float> AHeatFlowOptimizationActor::CalculateHeatFlux(float ThermalConductivityValue, const TArray<float>& TemperatureGradientArray, float MagicalFluxValue) const
{
	TArray<float> Result;
	Result.SetNum(TemperatureGradientArray.Num());
	for (int32 Index = 0; Index < TemperatureGradientArray.Num(); ++Index)
	{
		Result[Index] = (-ThermalConductivityValue * TemperatureGradientArray[Index]) + MagicalFluxValue;
	}
	return Result;
}

void AHeatFlowOptimizationActor::PredictHeatFluxWithNeuralNetwork()
{
	UE_LOG(LogTemp, Display, TEXT("Neural prediction placeholder executed."));
}

void AHeatFlowOptimizationActor::LogTensorFlowApproximation()
{
	UE_LOG(LogTemp, Display, TEXT("TensorFlow-style computation placeholder executed."));
}
