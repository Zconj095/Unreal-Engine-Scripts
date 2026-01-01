#include "FeedbackLoopBehavioralAnalysisComponent.h"
#include "Math/UnrealMathUtility.h"

UFeedbackLoopBehavioralAnalysisComponent::UFeedbackLoopBehavioralAnalysisComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InitialFeedback = 0.0f;
	LearningRate = 0.1f;
	ObservedBehavior = 1.0f;
	ExpectedBehavior = 0.8f;
}

void UFeedbackLoopBehavioralAnalysisComponent::BeginPlay()
{
	Super::BeginPlay();

	float FeedbackState = InitialFeedback;
	for (int32 Index = 0; Index < 10; ++Index)
	{
		FeedbackState = FeedbackState + LearningRate * (ObservedBehavior - ExpectedBehavior);
		UE_LOG(LogTemp, Log, TEXT("Iteration %d: Feedback State (F_n+1) = %.4f"), Index + 1, FeedbackState);
	}
}
