#include "FeedbackLoopBiosyntheticOptimizationComponent.h"

UFeedbackLoopBiosyntheticOptimizationComponent::UFeedbackLoopBiosyntheticOptimizationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InitialFeedback = 0.0f;
	OptimizationRate = 0.1f;
	ObservedOutput = 5.0f;
	ExpectedOutput = 4.5f;
	Iterations = 10;
}

void UFeedbackLoopBiosyntheticOptimizationComponent::BeginPlay()
{
	Super::BeginPlay();

	float FeedbackState = InitialFeedback;
	for (int32 Index = 0; Index < Iterations; ++Index)
	{
		FeedbackState = FeedbackState + OptimizationRate * (ObservedOutput - ExpectedOutput);
		UE_LOG(LogTemp, Log, TEXT("Iteration %d: Feedback State (F_%d) = %.4f"), Index + 1, Index + 1, FeedbackState);
	}
}
