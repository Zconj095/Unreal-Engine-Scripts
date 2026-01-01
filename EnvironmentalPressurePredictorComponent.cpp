#include "EnvironmentalPressurePredictorComponent.h"
#include "Math/UnrealMathUtility.h"

UEnvironmentalPressurePredictorComponent::UEnvironmentalPressurePredictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InputValue = 1.5f;
	Epochs = 1000;
	LearningRate = 0.01f;
}

void UEnvironmentalPressurePredictorComponent::BeginPlay()
{
	Super::BeginPlay();

	TrainPlaceholder();
	const float Prediction = PredictPressure(InputValue);
	UE_LOG(LogTemp, Log, TEXT("Predicted Pressure: %.2f"), Prediction);
}

float UEnvironmentalPressurePredictorComponent::PredictPressure(float Input) const
{
	return Input * LearningRate;
}

void UEnvironmentalPressurePredictorComponent::TrainPlaceholder() const
{
	for (int32 Index = 0; Index < Epochs; ++Index)
	{
		if (Index % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d running (placeholder)."), Index + 1, Epochs);
		}
	}
}
