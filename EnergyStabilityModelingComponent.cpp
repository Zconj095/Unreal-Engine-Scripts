#include "EnergyStabilityModelingComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyStabilityModelingComponent::UEnergyStabilityModelingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	EnergyFluctuation = 500.0;
	TimeInterval = 10.0;
	StabilityIndex = 0.0;
}

void UEnergyStabilityModelingComponent::BeginPlay()
{
	Super::BeginPlay();

	StabilityIndex = CalculateStabilityIndex(EnergyFluctuation, TimeInterval);
	UE_LOG(LogTemp, Log, TEXT("Stability Index (S): %f"), StabilityIndex);
	UE_LOG(LogTemp, Log, TEXT("Predicted Stability Index (S): %f"), PredictStabilityIndex(EnergyFluctuation, TimeInterval));
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (S): %f"), ComputeWithTensorFlow(EnergyFluctuation, TimeInterval));
}

double UEnergyStabilityModelingComponent::CalculateStabilityIndex(double DeltaE, double DeltaT) const
{
	if (FMath::IsNearlyZero(DeltaT))
	{
		UE_LOG(LogTemp, Error, TEXT("deltaT cannot be zero!"));
		return 0.0;
	}

	return DeltaE / DeltaT;
}

double UEnergyStabilityModelingComponent::PredictStabilityIndex(double DeltaE, double DeltaT) const
{
	return CalculateStabilityIndex(DeltaE, DeltaT) * 0.98; // placeholder estimate
}

double UEnergyStabilityModelingComponent::ComputeWithTensorFlow(double DeltaE, double DeltaT) const
{
	return CalculateStabilityIndex(DeltaE, DeltaT);
}
