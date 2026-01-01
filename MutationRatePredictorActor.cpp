#include "MutationRatePredictorActor.h"

AMutationRatePredictorActor::AMutationRatePredictorActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMutationRatePredictorActor::BeginPlay()
{
    Super::BeginPlay();

    LogPrediction();
}

void AMutationRatePredictorActor::LogPrediction() const
{
    if (Inputs.Num() == 0 || Outputs.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient data for neural prediction."));
        return;
    }

    // Log a simulated prediction result
    UE_LOG(LogTemp, Log, TEXT("Predicted Mutation Rate: %.4f"), Outputs[0]);
}
