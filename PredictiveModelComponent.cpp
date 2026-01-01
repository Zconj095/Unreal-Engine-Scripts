#include "PredictiveModelComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPredictiveModelComponent::UPredictiveModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPredictiveModelComponent::BeginPlay()
{
	Super::BeginPlay();
	PredictiveCapacity = CalculatePredictiveCapacity();
	UE_LOG(LogTemp, Log, TEXT("Predictive Capacity: %0.4f"), PredictiveCapacity);
}

float UPredictiveModelComponent::CalculatePredictiveCapacity() const
{
	if (IntegrationSteps <= 0 || ObservationPeriod <= 0.0f || StandardDeviation <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for predictive capacity."));
		return 0.0f;
	}

	const float Step = ObservationPeriod / IntegrationSteps;
	float Sum = 0.0f;

	for (int32 Index = 0; Index < IntegrationSteps; ++Index)
	{
		const float X = Index * Step;
		const float Coefficient = 1.0f / (FMath::Sqrt(2.0f * PI) * StandardDeviation);
		const float Exponent = -FMath::Square(X - Mean) / (2.0f * FMath::Square(StandardDeviation));
		const float Gaussian = Coefficient * FMath::Exp(Exponent);
		Sum += Gaussian * Step;
	}

	return Sum;
}
