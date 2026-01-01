#include "NeuroevolutionaryFeedbackLoopComponent.h"
#include "Logging/LogMacros.h"

UNeuroevolutionaryFeedbackLoopComponent::UNeuroevolutionaryFeedbackLoopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuroevolutionaryFeedbackLoopComponent::BeginPlay()
{
	Super::BeginPlay();

	SimulateFeedbackLoop();
}

void UNeuroevolutionaryFeedbackLoopComponent::SimulateFeedbackLoop()
{
	CurrentFeedback = InitialFeedback;
	UE_LOG(LogTemp, Log, TEXT("Iteration 0: Feedback = %0.4f"), CurrentFeedback);

	for (int32 Iteration = 1; Iteration <= Iterations; ++Iteration)
	{
		CurrentFeedback += LearningRate * (ObservedAdaptation - ExpectedAdaptation);
		UE_LOG(LogTemp, Log, TEXT("Iteration %d: Feedback = %0.4f"), Iteration, CurrentFeedback);
	}
}
