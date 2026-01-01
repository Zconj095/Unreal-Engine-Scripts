#include "LunarFeedbackLoopComponent.h"
#include "Engine/Engine.h"

ULunarFeedbackLoopComponent::ULunarFeedbackLoopComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULunarFeedbackLoopComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentFeedback = InitialFeedback;
	LogFeedback();
}

void ULunarFeedbackLoopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float NextFeedback = CurrentFeedback + AdaptationRate * (ObservedPhenomena - ExpectedPhenomena);
	CurrentFeedback = NextFeedback;
	LogFeedback();
}

void ULunarFeedbackLoopComponent::LogFeedback() const
{
	UE_LOG(LogTemp, Log, TEXT("Updated Feedback (Fn+1): %.2f"), CurrentFeedback);
}
