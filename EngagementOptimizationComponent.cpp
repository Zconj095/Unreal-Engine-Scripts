#include "EngagementOptimizationComponent.h"
#include "Math/UnrealMathUtility.h"

UEngagementOptimizationComponent::UEngagementOptimizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PlayerSkill = 75.0;
	ChallengeComplexity = 50.0;
	FlowState = 0.0;
}

void UEngagementOptimizationComponent::BeginPlay()
{
	Super::BeginPlay();

	FlowState = CalculateFlowState(PlayerSkill, ChallengeComplexity);
	UE_LOG(LogTemp, Log, TEXT("Flow State Intensity (F): %f"), FlowState);
	UE_LOG(LogTemp, Log, TEXT("Predicted Flow State Intensity (F): %f"), PredictFlowState(PlayerSkill, ChallengeComplexity));
	UE_LOG(LogTemp, Log, TEXT("TensorFlow Computation Result (F): %f"), ComputeWithTensorFlow(PlayerSkill, ChallengeComplexity));
}

double UEngagementOptimizationComponent::CalculateFlowState(double Skill, double Complexity) const
{
	if (FMath::IsNearlyZero(Complexity))
	{
		UE_LOG(LogTemp, Error, TEXT("Challenge Complexity (C) cannot be zero!"));
		return 0.0;
	}
	return Skill / Complexity;
}

double UEngagementOptimizationComponent::PredictFlowState(double Skill, double Complexity) const
{
	return CalculateFlowState(Skill, Complexity) * 0.95;
}

double UEngagementOptimizationComponent::ComputeWithTensorFlow(double Skill, double Complexity) const
{
	return CalculateFlowState(Skill, Complexity);
}
