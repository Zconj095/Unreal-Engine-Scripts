#include "EvolutionaryFeedbackLoopComponent.h"
#include "Math/UnrealMathUtility.h"

UEvolutionaryFeedbackLoopComponent::UEvolutionaryFeedbackLoopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InitialFeedback = 0.0f;
	AdaptationRate = 0.1f;
	ObservedOutcome = 1.0f;
	ExpectedOutcome = 0.8f;
	Iterations = 10;
}

void UEvolutionaryFeedbackLoopComponent::BeginPlay()
{
	Super::BeginPlay();

	float CurrentFeedback = InitialFeedback;
	UE_LOG(LogTemp, Log, TEXT("Iteration 0: Feedback = %f"), CurrentFeedback);

	for (int32 Index = 1; Index <= Iterations; ++Index)
	{
		CurrentFeedback = CurrentFeedback + AdaptationRate * (ObservedOutcome - ExpectedOutcome);
		UE_LOG(LogTemp, Log, TEXT("Iteration %d: Feedback = %f"), Index, CurrentFeedback);
	}
}
