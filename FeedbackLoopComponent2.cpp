#include "FeedbackLoopComponent2.h"
#include "Math/UnrealMathUtility.h"

UFeedbackLoopComponent2::UFeedbackLoopComponent2()
{
	PrimaryComponentTick.bCanEverTick = true;
	OptimizationRate = 0.1f;
	InitialFeedbackState = 0.0f;
	ExpectedEfficiency = 1.0f;
	CurrentFeedbackState = InitialFeedbackState;
}

void UFeedbackLoopComponent2::BeginPlay()
{
	Super::BeginPlay();
	CurrentFeedbackState = InitialFeedbackState;
}

void UFeedbackLoopComponent2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	const float ObservedEfficiency = FMath::Sin(CurrentTime) + 1.0f;
	CurrentFeedbackState += OptimizationRate * (ObservedEfficiency - ExpectedEfficiency);
	UE_LOG(LogTemp, Log, TEXT("Feedback State (Fn+1): %.2f"), CurrentFeedbackState);
}
