#include "FeedbackLoopCalculatorComponent.h"
#include "Math/UnrealMathUtility.h"

UFeedbackLoopCalculatorComponent::UFeedbackLoopCalculatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Fn = 1.0f;
	Beta = 0.1f;
	Og = 10.0f;
	Eg = 8.0f;
}

void UFeedbackLoopCalculatorComponent::BeginPlay()
{
	Super::BeginPlay();

	const float UpdatedFeedback = Fn + Beta * (Og - Eg);
	UE_LOG(LogTemp, Log, TEXT("Updated Feedback State (F_n+1): %.2f"), UpdatedFeedback);
	Fn = UpdatedFeedback;
}

void UFeedbackLoopCalculatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Fn = Fn + Beta * (Og - Eg);
	UE_LOG(LogTemp, Log, TEXT("New Feedback State (F_n): %.2f"), Fn);
}
