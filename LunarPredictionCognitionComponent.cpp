#include "LunarPredictionCognitionComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULunarPredictionCognitionComponent::ULunarPredictionCognitionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarPredictionCognitionComponent::BeginPlay()
{
	Super::BeginPlay();
	Prediction = CalculatePredictionCognition(0.0f);
	LogPrediction();
}

void ULunarPredictionCognitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float Time = GetWorld()->GetTimeSeconds();
	Prediction = CalculatePredictionCognition(Time);
	LogPrediction();
}

float ULunarPredictionCognitionComponent::CalculatePredictionCognition(float Time) const
{
	if (IntegrationSteps <= 0)
	{
		return 0.0f;
	}

	const float DeltaX = ObservationPeriod / IntegrationSteps;
	float Total = 0.0f;

	for (int32 Index = 0; Index < IntegrationSteps; ++Index)
	{
		const float X = Index * DeltaX;
		const float ProbabilityDensity = (1.0f / (FMath::Sqrt(2.0f * PI) * StandardDeviation)) *
			FMath::Exp(-FMath::Pow(X - Mean, 2) / (2.0f * StandardDeviation * StandardDeviation));
		Total += ProbabilityDensity * DeltaX;
	}

	return Total;
}

void ULunarPredictionCognitionComponent::LogPrediction() const
{
	UE_LOG(LogTemp, Log, TEXT("Lunar Prediction Cognition: %.2f"), Prediction);
}
