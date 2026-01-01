#include "LeylineSolarIntegrationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineSolarIntegrationComponent::ULeylineSolarIntegrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineSolarIntegrationComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateSolarPower();
}

double ULeylineSolarIntegrationComponent::Phi(double Time) const
{
	return FMath::Sin(Time) * FMath::Exp(-0.1 * Time);
}

double ULeylineSolarIntegrationComponent::GradientPsi(double Time) const
{
	return FMath::Cos(Time) * FMath::Exp(-0.05 * Time);
}

double ULeylineSolarIntegrationComponent::ComputeSolarPower() const
{
	if (IntegrationSteps <= 0 || EndTime <= StartTime)
	{
		return 0.0;
	}

	const double Step = (EndTime - StartTime) / IntegrationSteps;
	double Sum = 0.0;

	for (int32 Index = 0; Index <= IntegrationSteps; ++Index)
	{
		const double Time = StartTime + Index * Step;
		Sum += Phi(Time) * GradientPsi(Time);
	}

	return Sum * Step;
}

void ULeylineSolarIntegrationComponent::LogPower(double Value) const
{
	UE_LOG(LogTemp, Log, TEXT("Power transferred to leyline systems: %.4f"), Value);
}

void ULeylineSolarIntegrationComponent::RecalculateSolarPower()
{
	SolarPower = ComputeSolarPower();
	LogPower(SolarPower);
}
