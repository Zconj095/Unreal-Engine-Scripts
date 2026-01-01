#include "EnvironmentalNeuralApproximatorComponent.h"
#include "Math/UnrealMathUtility.h"

UEnvironmentalNeuralApproximatorComponent::UEnvironmentalNeuralApproximatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InputSample = {100.0f, 200.0f, 150.0f};
	Epochs = 1000;
	LearningRate = 0.01f;
}

void UEnvironmentalNeuralApproximatorComponent::BeginPlay()
{
	Super::BeginPlay();

	TrainPlaceholder();
	const float Prediction = PredictSystemStrength(InputSample);
	UE_LOG(LogTemp, Log, TEXT("Predicted System Strength (S): %.2f"), Prediction);
}

float UEnvironmentalNeuralApproximatorComponent::PredictSystemStrength(const TArray<float>& Inputs) const
{
	float Sum = 0.0f;
	for (float Value : Inputs)
	{
		Sum += Value;
	}
	return Sum * LearningRate;
}

void UEnvironmentalNeuralApproximatorComponent::TrainPlaceholder() const
{
	for (int32 Index = 0; Index < Epochs; ++Index)
	{
		if (Index % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d running (placeholder)."), Index + 1, Epochs);
		}
	}
}
