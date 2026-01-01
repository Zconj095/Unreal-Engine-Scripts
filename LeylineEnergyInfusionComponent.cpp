#include "LeylineEnergyInfusionComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusionComponent::ULeylineEnergyInfusionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyInfusionComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLeylineEnergy();
}

double ULeylineEnergyInfusionComponent::Phi(double Time) const
{
	return FMath::Sin(Time) + 1.0;
}

double ULeylineEnergyInfusionComponent::GradPsi(double Time) const
{
	return FMath::Cos(Time) + 1.0;
}

double ULeylineEnergyInfusionComponent::IntegrateLeylineEnergy() const
{
	if (EndTime <= StartTime || TimeStep <= 0.0)
	{
		return 0.0;
	}

	double Sum = 0.0;
	for (double t = StartTime; t <= EndTime; t += TimeStep)
	{
		const double Value = Phi(t) * GradPsi(t);
		Sum += Value * TimeStep;
	}
	return Sum;
}

void ULeylineEnergyInfusionComponent::RecalculateLeylineEnergy()
{
	InfusedPower = IntegrateLeylineEnergy();
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Transferred (P_invention): %.4f"), InfusedPower);
	UE_LOG(LogTemp, Log, TEXT("Predicted Leyline Energy Transferred (P_invention): %.4f"), PredictLeylineEnergyWithMockNetwork());
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (P_invention): %.4f"), TensorFlowPlaceholder());
}

double ULeylineEnergyInfusionComponent::PredictLeylineEnergyWithMockNetwork() const
{
	return InfusedPower * 0.98;
}

double ULeylineEnergyInfusionComponent::TensorFlowPlaceholder() const
{
	const double Result = IntegrateLeylineEnergy();
	return Result;
}
