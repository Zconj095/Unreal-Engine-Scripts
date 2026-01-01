#include "EnergyHarmonizationComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyHarmonizationComponent::UEnergyHarmonizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	GradientEnergyFlow = 1.0f;
	TemporalEnergyDistribution = 1.0f;
	MagicalEnergyHarmonization = 0.0f;
	IntegrationStart = 0.0f;
	IntegrationEnd = 10.0f;
	EnergyBalance = 0.0f;
}

void UEnergyHarmonizationComponent::BeginPlay()
{
	Super::BeginPlay();

	EnergyBalance = CalculateEnergyBalance(GradientEnergyFlow, TemporalEnergyDistribution, MagicalEnergyHarmonization, IntegrationStart, IntegrationEnd);
	UE_LOG(LogTemp, Log, TEXT("Energy Balance (Be): %.4f"), EnergyBalance);
	TensorFlowValidation(GradientEnergyFlow, TemporalEnergyDistribution, MagicalEnergyHarmonization, IntegrationStart, IntegrationEnd);
}

double UEnergyHarmonizationComponent::TrapezoidalIntegration(TFunctionRef<double(double)> Function, double Start, double End, int32 Steps) const
{
	const double H = (End - Start) / Steps;
	double Sum = 0.5 * (Function(Start) + Function(End));
	for (int32 Index = 1; Index < Steps; ++Index)
	{
		Sum += Function(Start + Index * H);
	}
	return H * Sum;
}

float UEnergyHarmonizationComponent::CalculateEnergyBalance(float GradientEnergy, float TemporalDistribution, float MagicalEnergy, float StartTime, float EndTime) const
{
	const auto Integrand = [&](double) { return FMath::Abs(GradientEnergy / TemporalDistribution); };
	const double IntegratedValue = TrapezoidalIntegration(Integrand, StartTime, EndTime, 1000);
	return static_cast<float>(IntegratedValue) + MagicalEnergy;
}

void UEnergyHarmonizationComponent::TensorFlowValidation(float GradientEnergy, float TemporalDistribution, float MagicalEnergy, float StartTime, float EndTime) const
{
	const double Result = CalculateEnergyBalance(GradientEnergy, TemporalDistribution, MagicalEnergy, StartTime, EndTime);
	UE_LOG(LogTemp, Log, TEXT("[TensorFlow Validation] Energy Balance (Be): %.4f"), Result);
}
