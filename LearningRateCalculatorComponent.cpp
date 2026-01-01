#include "LearningRateCalculatorComponent.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

ULearningRateCalculatorComponent::ULearningRateCalculatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULearningRateCalculatorComponent::BeginPlay()
{
	Super::BeginPlay();
	RecalculateLearningRate();
}

void ULearningRateCalculatorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RecalculateLearningRate();
}

float ULearningRateCalculatorComponent::EvaluateLearningRate(float L0, float Beta, float Qc, float Qt) const
{
	const float Denominator = 1.0f + FMath::Exp(-Beta * (Qc - Qt));
	return FMath::IsNearlyZero(Denominator) ? L0 : L0 / Denominator;
}

void ULearningRateCalculatorComponent::LogLearningRate() const
{
	UE_LOG(LogTemp, Log, TEXT("Learning Rate: %.4f"), LearningRate);
}

void ULearningRateCalculatorComponent::RecalculateLearningRate()
{
	LearningRate = EvaluateLearningRate(MaxLearningRate, ScalingFactor, QuantumComplexity, ThresholdComplexity);
	LogLearningRate();
}
