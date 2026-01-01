#include "EnergeticAlignmentDynamicsComponent.h"
#include "Math/UnrealMathUtility.h"

UEnergeticAlignmentDynamicsComponent::UEnergeticAlignmentDynamicsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CosmicForce = FVector2D(10.0f, 5.0f);
	StartTime = 0.0f;
	EndTime = 10.0f;
	TimeStep = 0.01f;
	EnergyFlow = 0.0f;
}

void UEnergeticAlignmentDynamicsComponent::BeginPlay()
{
	Super::BeginPlay();

	EnergyFlow = CalculateEnergyFlow(CosmicForce, StartTime, EndTime, TimeStep);
	UE_LOG(LogTemp, Log, TEXT("Energy Flow Φ during alignment: %f"), EnergyFlow);

	TArray<float> Prediction = PredictEnergyFlow(CosmicForce, EndTime);
	if (Prediction.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Predicted Energy Flow Φ: %f"), Prediction[0]);
	}

	float TfResult = ComputeWithTensorFlow(CosmicForce, StartTime, EndTime, TimeStep);
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result Φ: %f"), TfResult);
}

FVector2D UEnergeticAlignmentDynamicsComponent::SampleEnergyGradient(float Time) const
{
	return FVector2D(FMath::Cos(Time), FMath::Sin(Time));
}

float UEnergeticAlignmentDynamicsComponent::CalculateEnergyFlow(const FVector2D& Force, float TStart, float TEnd, float Dt) const
{
	if (Dt <= 0.0f || TEnd <= TStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid integration bounds or delta for EnergeticAlignmentDynamicsComponent."));
		return 0.0f;
	}

	float TotalEnergyFlow = 0.0f;
	for (float t = TStart; t <= TEnd; t += Dt)
	{
		FVector2D Grad = SampleEnergyGradient(t);
		TotalEnergyFlow += FVector2D::DotProduct(Force, Grad) * Dt;
	}

	return TotalEnergyFlow;
}

TArray<float> UEnergeticAlignmentDynamicsComponent::PredictEnergyFlow(const FVector2D& Force, float TimeEnd) const
{
	float Sum = Force.X * 0.6f + Force.Y * 0.4f + TimeEnd * 0.05f;
	const float Sigmoid = 1.0f / (1.0f + FMath::Exp(-Sum));
	return { Sigmoid };
}

float UEnergeticAlignmentDynamicsComponent::ComputeWithTensorFlow(const FVector2D& Force, float TStart, float TEnd, float Dt) const
{
	// Placeholder stub returning the same numerical integration result.
	return CalculateEnergyFlow(Force, TStart, TEnd, Dt);
}
