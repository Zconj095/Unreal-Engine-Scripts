#include "PredictionOfAtomizationComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPredictionOfAtomizationComponent::UPredictionOfAtomizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPredictionOfAtomizationComponent::BeginPlay()
{
	Super::BeginPlay();

	PredictiveCapability = CalculatePredictiveCapability();
	UE_LOG(LogTemp, Log, TEXT("Predictive Capability (Pa): %0.4f"), PredictiveCapability);
}

float UPredictionOfAtomizationComponent::GaussianFunction(float Value, float InMean, float InStdDev) const
{
	const float Coefficient = 1.0f / FMath::Sqrt(2.0f * PI * FMath::Square(InStdDev));
	const float Exponent = -FMath::Square(Value - InMean) / (2.0f * FMath::Square(InStdDev));
	return Coefficient * FMath::Exp(Exponent);
}

float UPredictionOfAtomizationComponent::CalculatePredictiveCapability() const
{
	if (IntegrationSteps <= 0 || ObservationPeriod <= 0.0f || FMath::IsNearlyZero(StdDev))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for predictive capability."));
		return 0.0f;
	}

	const float Dt = ObservationPeriod / IntegrationSteps;
	float Result = 0.0f;

	for (int32 Index = 0; Index < IntegrationSteps; ++Index)
	{
		const float X1 = Index * Dt;
		const float X2 = (Index + 1) * Dt;
		const float Y1 = GaussianFunction(X1, Mean, StdDev);
		const float Y2 = GaussianFunction(X2, Mean, StdDev);
		Result += 0.5f * (Y1 + Y2) * Dt;
	}

	return Result;
}
