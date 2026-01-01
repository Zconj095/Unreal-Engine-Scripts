#include "LeylineNeuralGradientComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULeylineNeuralGradientComponent::ULeylineNeuralGradientComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULeylineNeuralGradientComponent::BeginPlay()
{
	Super::BeginPlay();
	LogSamplePrediction();
}

double ULeylineNeuralGradientComponent::EvaluateGradient(double Time) const
{
	return 1.0 / (1.0 + FMath::Exp(-Time));
}

void ULeylineNeuralGradientComponent::LogSamplePrediction()
{
	PredictedGradient = EvaluateGradient(5.0);
	UE_LOG(LogTemp, Log, TEXT("Predicted gradient of leyline energy flow at t=5.0: %.4f"), PredictedGradient);
}
