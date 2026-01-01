#include "GeneticFeedbackLoopActor.h"

#include "Math/UnrealMathUtility.h"

AGeneticFeedbackLoopActor::AGeneticFeedbackLoopActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneticFeedbackLoopActor::BeginPlay()
{
	Super::BeginPlay();
	SimulateFeedbackLoop();
}

void AGeneticFeedbackLoopActor::SimulateFeedbackLoop()
{
	float CurrentFeedback = InitialFeedback;
	UE_LOG(LogTemp, Display, TEXT("Iteration 0: Feedback = %.4f"), CurrentFeedback);

	for (int32 Iter = 1; Iter <= Iterations; ++Iter)
	{
		CurrentFeedback = CurrentFeedback + LearningRate * (ObservedOutcome - ExpectedOutcome);
		UE_LOG(LogTemp, Display, TEXT("Iteration %d: Feedback = %.4f"), Iter, CurrentFeedback);
	}
}
