#include "LeylineEnergyInfusion3Component.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyInfusion3Component::ULeylineEnergyInfusion3Component()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyInfusion3Component::BeginPlay()
{
	Super::BeginPlay();
	RecalculateNeuralEnergy();
}

double ULeylineEnergyInfusion3Component::MagicalEnergyField(double Time) const
{
	return FMath::Sin(Time) + 2.0;
}

double ULeylineEnergyInfusion3Component::EnergyGradientField(double Time) const
{
	return FMath::Cos(Time) + 1.0;
}

double ULeylineEnergyInfusion3Component::CalculateNeuralEnergyPower() const
{
	const int32 Points = FMath::Max(IntegrationPoints, 10);
	const double Step = (EndTime - StartTime) / Points;
	double Accumulated = 0.0;

	for (int32 Index = 0; Index <= Points; ++Index)
	{
		const double Time = StartTime + Index * Step;
		Accumulated += MagicalEnergyField(Time) * EnergyGradientField(Time);
	}

	return Accumulated * Step;
}

void ULeylineEnergyInfusion3Component::RecalculateNeuralEnergy()
{
	NeuralEnergyPower = CalculateNeuralEnergyPower();
	UE_LOG(LogTemp, Log, TEXT("Leyline Neural Power (P_neural): %.4f"), NeuralEnergyPower);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (P_neural): %.4f"), TensorFlowPlaceholder());
}

double ULeylineEnergyInfusion3Component::TensorFlowPlaceholder() const
{
	return CalculateNeuralEnergyPower();
}
