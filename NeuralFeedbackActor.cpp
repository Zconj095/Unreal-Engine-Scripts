#include "NeuralFeedbackActor.h"

ANeuralFeedbackActor::ANeuralFeedbackActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralFeedbackActor::BeginPlay()
{
    Super::BeginPlay();

    float FeedbackState = InitialFeedback;
    FeedbackState = UpdateFeedback(FeedbackState);
    UE_LOG(LogTemp, Log, TEXT("Updated Feedback State (F_n+1): %.4f"), FeedbackState);
}

float ANeuralFeedbackActor::UpdateFeedback(float CurrentFeedback) const
{
    return CurrentFeedback + LearningRate * (ObservedPhenomena - ExpectedBehavior);
}
