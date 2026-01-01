#include "LeylineEnergyTransferComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyTransferComponent::ULeylineEnergyTransferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyTransferComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLeylinePower();
}

double ULeylineEnergyTransferComponent::EvaluatePhi(double Time) const
{
	return FMath::Sin(Time) + 2.0;
}

double ULeylineEnergyTransferComponent::EvaluateGradPsi(double Time) const
{
	return FMath::Cos(Time) + 1.0;
}

double ULeylineEnergyTransferComponent::IntegrateLeylinePower() const
{
	if (EndTime <= StartTime || TimeStep <= 0.0)
	{
		return 0.0;
	}

	double Sum = 0.0;
	for (double Time = StartTime; Time < EndTime; Time += TimeStep)
	{
		Sum += EvaluatePhi(Time) * EvaluateGradPsi(Time) * TimeStep;
	}

	return Sum;
}

void ULeylineEnergyTransferComponent::RecalculateLeylinePower()
{
	LeylinePower = IntegrateLeylinePower();
	UE_LOG(LogTemp, Log, TEXT("P_leyline (Power transferred via leyline): %.4f"), LeylinePower);
	UE_LOG(LogTemp, Log, TEXT("Predicted Leyline Power (mock): %.4f"), LeylinePower * 1.05);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (P_leyline placeholder): %.4f"), LeylinePower);
}
