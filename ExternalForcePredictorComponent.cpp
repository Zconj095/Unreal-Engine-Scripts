#include "ExternalForcePredictorComponent.h"
#include "Math/UnrealMathUtility.h"

UExternalForcePredictorComponent::UExternalForcePredictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InputPosition = FVector(5.0f, 5.0f, 0.0f);
	Epochs = 1000;
	LearningRate = 0.01f;
}

void UExternalForcePredictorComponent::BeginPlay()
{
	Super::BeginPlay();

	TrainPlaceholder();
	float Fx = PredictForce(InputPosition);
	float Fy = PredictForce(InputPosition * FVector(1.0f, 1.0f, 0.0f));
	UE_LOG(LogTemp, Log, TEXT("Predicted Force: Fx = %.4f, Fy = %.4f"), Fx, Fy);
}

float UExternalForcePredictorComponent::PredictForce(const FVector& Position) const
{
	// Simplified placeholder: force magnitude falls with distance squared and constant gravity (-9.81)
	const float Distance = Position.Size();
	const float Gravity = -9.81f;
	return Gravity / (1.0f + Distance * Distance) * LearningRate;
}

void UExternalForcePredictorComponent::TrainPlaceholder() const
{
	for (int32 Index = 0; Index < Epochs; ++Index)
	{
		if (Index % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d running (placeholder)."), Index + 1, Epochs);
		}
	}
}
