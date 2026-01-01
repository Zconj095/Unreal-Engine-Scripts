#include "FeedbackLoopComponent.h"
#include "Math/UnrealMathUtility.h"

UFeedbackLoopComponent::UFeedbackLoopComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Beta = 0.1f;
	ExpectedOutput = 1.0f;
	InitialState = 0.0f;
	ObservedOutput = 0.0f;
	CurrentState = InitialState;
	UpdatedState = InitialState;
}

void UFeedbackLoopComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentState = InitialState;
}

void UFeedbackLoopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatedState = CurrentState + Beta * (ObservedOutput - ExpectedOutput);
	CurrentState = UpdatedState;

	UE_LOG(LogTemp, Log, TEXT("Updated Feedback State (Fn+1): %.2f"), UpdatedState);
}
