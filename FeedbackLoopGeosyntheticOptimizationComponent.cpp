#include "FeedbackLoopGeosyntheticOptimizationComponent.h"

UFeedbackLoopGeosyntheticOptimizationComponent::UFeedbackLoopGeosyntheticOptimizationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	OptimizationRate = 0.1f;
	InitialFeedbackState = 1.0f;
	ObservedOutput = 2.0f;
	ExpectedOutput = 1.5f;
	CurrentFeedbackState = 0.0f;
	UpdatedFeedbackState = 0.0f;
}

void UFeedbackLoopGeosyntheticOptimizationComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentFeedbackState = InitialFeedbackState;
}

void UFeedbackLoopGeosyntheticOptimizationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatedFeedbackState = CurrentFeedbackState + OptimizationRate * (ObservedOutput - ExpectedOutput);
	UE_LOG(LogTemp, Log, TEXT("Current Feedback State: %.2f, Updated Feedback State: %.2f"), CurrentFeedbackState, UpdatedFeedbackState);
	CurrentFeedbackState = UpdatedFeedbackState;
}
