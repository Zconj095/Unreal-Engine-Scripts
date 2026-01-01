#include "LudicLeylineEnergyTransferComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULudicLeylineEnergyTransferComponent::ULudicLeylineEnergyTransferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULudicLeylineEnergyTransferComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLudicPower();
}

double ULudicLeylineEnergyTransferComponent::Phi(double Time) const
{
	return FMath::Sin(Time) + 1.0;
}

double ULudicLeylineEnergyTransferComponent::GradPsi(double Time) const
{
	return FMath::Cos(Time) + 1.0;
}

double ULudicLeylineEnergyTransferComponent::IntegrateLudicPower() const
{
	if (EndTime <= StartTime || TimeStep <= 0.0)
	{
		return 0.0;
	}

	double Sum = 0.0;
	for (double Time = StartTime; Time <= EndTime; Time += TimeStep)
	{
		Sum += Phi(Time) * GradPsi(Time) * TimeStep;
	}

	return Sum;
}

void ULudicLeylineEnergyTransferComponent::RecalculateLudicPower()
{
	LudicPower = IntegrateLudicPower();
	UE_LOG(LogTemp, Log, TEXT("Ludic Energy Transfer (P_ludic): %.4f"), LudicPower);
	UE_LOG(LogTemp, Log, TEXT("Predicted Ludic Energy Transfer (mock): %.4f"), LudicPower * 0.93);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (mock): %.4f"), LudicPower);
}
