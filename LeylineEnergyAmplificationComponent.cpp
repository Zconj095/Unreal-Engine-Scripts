#include "LeylineEnergyAmplificationComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineEnergyAmplificationComponent::ULeylineEnergyAmplificationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineEnergyAmplificationComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateAmplification();
}

double ULeylineEnergyAmplificationComponent::EvaluateCurve(UCurveFloat* Curve, float Time) const
{
	if (Curve)
	{
		return Curve->GetFloatValue(Time);
	}

	return 0.0;
}

double ULeylineEnergyAmplificationComponent::ComputeLeylineEnergyAmplification() const
{
	const int32 Steps = FMath::Max(IntegrationSteps, 1);
	const double StepSize = (IntegrationEnd - IntegrationStart) / Steps;
	double Result = 0.0;

	for (int32 Index = 0; Index <= Steps; ++Index)
	{
		const float Time = IntegrationStart + Index * StepSize;
		const double Phi = EvaluateCurve(MagicalEnergyDensityCurve, Time);
		const double Gradient = EvaluateCurve(LeylineEnergyGradientCurve, Time);
		Result += Phi * Gradient * StepSize;
	}

	return Result;
}

void ULeylineEnergyAmplificationComponent::TensorFlowValidation() const
{
	const int32 Steps = FMath::Max(IntegrationSteps, 1);
	const double StepSize = (IntegrationEnd - IntegrationStart) / Steps;
	double Sum = 0.0;

	for (int32 Index = 0; Index <= Steps; ++Index)
	{
		const float Time = IntegrationStart + Index * StepSize;
		const double Phi = EvaluateCurve(MagicalEnergyDensityCurve, Time);
		const double Gradient = EvaluateCurve(LeylineEnergyGradientCurve, Time);
		Sum += Phi * Gradient;
	}

	const double LocalAmplifiedEnergy = Sum * StepSize;
	UE_LOG(LogTemp, Log, TEXT("[TensorFlow Validation] Leyline Energy Amplification: %.4f"), LocalAmplifiedEnergy);
}

void ULeylineEnergyAmplificationComponent::RecalculateAmplification()
{
	AmplifiedEnergy = ComputeLeylineEnergyAmplification();
	UE_LOG(LogTemp, Log, TEXT("Leyline Energy Amplification: %.4f"), AmplifiedEnergy);
	TensorFlowValidation();
}
