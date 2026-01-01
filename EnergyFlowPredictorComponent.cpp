#include "EnergyFlowPredictorComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergyFlowPredictorComponent::UEnergyFlowPredictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InputSize = 4;
	OutputSize = 1;
	LearningRate = 0.01f;
	Epochs = 1000;
}

void UEnergyFlowPredictorComponent::BeginPlay()
{
	Super::BeginPlay();

	TrainPlaceholder();

	TArray<float> Prediction = PredictPlaceholder({550.0f, 0.015f, 325.0f, 55.0f});
	if (Prediction.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Energy Input: %.2f"), Prediction[0]);
	}
}

void UEnergyFlowPredictorComponent::TrainPlaceholder() const
{
	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		if (Epoch % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d running (placeholder)."), Epoch + 1, Epochs);
		}
	}
}

TArray<float> UEnergyFlowPredictorComponent::PredictPlaceholder(const TArray<float>& Inputs) const
{
	float Sum = 0.0f;
	for (float Value : Inputs)
	{
		Sum += Value;
	}
	return { Sum * LearningRate };
}
