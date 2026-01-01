#include "PredictiveSeasonalModelingComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPredictiveSeasonalModelingComponent::UPredictiveSeasonalModelingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPredictiveSeasonalModelingComponent::BeginPlay()
{
	Super::BeginPlay();
	PredictiveCapability = CalculatePredictiveCapability();
	UE_LOG(LogTemp, Log, TEXT("Predictive Capability (Ps): %0.4f"), PredictiveCapability);
}

float UPredictiveSeasonalModelingComponent::GaussianFunction(float X) const
{
	const float Coefficient = 1.0f / FMath::Sqrt(2.0f * PI * FMath::Square(Sigma));
	const float Exponent = -FMath::Square(X - Mean) / (2.0f * FMath::Square(Sigma));
	return Coefficient * FMath::Exp(Exponent);
}

float UPredictiveSeasonalModelingComponent::CalculatePredictiveCapability() const
{
	if (Resolution <= 0 || ObservationPeriod <= 0.0f || Sigma <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for predictive capability."));
		return 0.0f;
	}

	const float Dt = ObservationPeriod / Resolution;
	float Total = 0.0f;

	for (int32 Index = 0; Index < Resolution; ++Index)
	{
		const float X1 = Index * Dt;
		const float X2 = (Index + 1) * Dt;
		const float F1 = GaussianFunction(X1);
		const float F2 = GaussianFunction(X2);
		Total += 0.5f * (F1 + F2) * Dt;
	}

	return Total;
}
