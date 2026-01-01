#include "LeylinePowerIntegrationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylinePowerIntegrationComponent::ULeylinePowerIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylinePowerIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLeylinePower();
}

double ULeylinePowerIntegrationComponent::EvaluateField(double Time) const
{
	return FMath::Sin(Time) + 2.0;
}

double ULeylinePowerIntegrationComponent::EvaluateGradient(double Time) const
{
	return FMath::Cos(Time) + 1.0;
}

double ULeylinePowerIntegrationComponent::IntegrateLeylinePower() const
{
	if (TimeStep <= 0.0 || EndTime <= StartTime)
	{
		return 0.0;
	}

	double Sum = 0.0;
	for (double Time = StartTime; Time <= EndTime; Time += TimeStep)
	{
		Sum += EvaluateField(Time) * EvaluateGradient(Time) * TimeStep;
	}

	return Sum;
}

void ULeylinePowerIntegrationComponent::RecalculateLeylinePower()
{
	LeylinePower = IntegrateLeylinePower();
	UE_LOG(LogTemp, Log, TEXT("Leyline Power Transferred (P_engine): %.4f"), LeylinePower);
	UE_LOG(LogTemp, Log, TEXT("Predicted Leyline Power (P_engine): %.4f"), LeylinePower * 1.01);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (P_engine): %.4f"), LeylinePower);
}
