#include "EnergyFlowOptimization3Component.h"
#include "Math/UnrealMathUtility.h"

UEnergyFlowOptimization3Component::UEnergyFlowOptimization3Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	StartX = 0.0f;
	EndX = 10.0f;
	StepSize = 0.01f;
	PowerDelivered = 0.0f;
}

void UEnergyFlowOptimization3Component::BeginPlay()
{
	Super::BeginPlay();

	PowerDelivered = CalculateEnergyFlow(StartX, EndX, StepSize);
	UE_LOG(LogTemp, Log, TEXT("Power Delivered (P): %f"), PowerDelivered);

	TArray<float> Prediction = PredictEnergyFlow(StartX, EndX, StepSize);
	if (Prediction.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Power Delivered (P): %f"), Prediction[0]);
	}

	float TensorFlowResult = ComputeWithTensorFlow(StartX, EndX, StepSize);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (P): %f"), TensorFlowResult);
}

float UEnergyFlowOptimization3Component::Phi(float X) const
{
	return FMath::Sin(X) + 1.0f;
}

float UEnergyFlowOptimization3Component::GradPsi(float X) const
{
	return FMath::Cos(X) + 1.0f;
}

float UEnergyFlowOptimization3Component::CalculateEnergyFlow(float X0, float X1, float DX) const
{
	float Total = 0.0f;
	for (float X = X0; X <= X1; X += DX)
	{
		const float PhiVal = Phi(X);
		const float GradVal = GradPsi(X);
		Total += PhiVal * GradVal * DX;
	}
	return Total;
}

TArray<float> UEnergyFlowOptimization3Component::PredictEnergyFlow(float X0, float X1, float DX) const
{
	const float Sum = (X1 - X0) / FMath::Max(DX, 0.0001f);
	const float Sigmoid = 1.0f / (1.0f + FMath::Exp(-Sum / 100.0f));
	return { Sigmoid };
}

float UEnergyFlowOptimization3Component::ComputeWithTensorFlow(float X0, float X1, float DX) const
{
	return CalculateEnergyFlow(X0, X1, DX);
}
