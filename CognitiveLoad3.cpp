#include "CognitiveLoad3.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoad3);

UCognitiveLoad3::UCognitiveLoad3()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveLoad3::BeginPlay()
{
    Super::BeginPlay();
    PreviousTime = GetWorld()->GetTimeSeconds();
}

void UCognitiveLoad3::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float CurrentTime = GetWorld()->GetTimeSeconds();
    AccumulateCognitiveLoad(CurrentTime);
    PreviousTime = CurrentTime;

    const float NormalizedLoad = ProcessingTimeConstant > KINDA_SMALL_NUMBER ? CognitiveLoadValue / ProcessingTimeConstant : 0.f;
    UE_LOG(LogCognitiveLoad3, Verbose, TEXT("Cognitive Load (Ln): %.2f"), NormalizedLoad);
}

float UCognitiveLoad3::CalculateInteractionFunction(float TimeSeconds) const
{
    float NeuralValue = NeuralStimuliOverTime ? NeuralStimuliOverTime->GetFloatValue(TimeSeconds) : 0.f;
    float BiochemicalValue = BiochemicalStimuliOverTime ? BiochemicalStimuliOverTime->GetFloatValue(TimeSeconds) : 0.f;
    return NeuralValue * BiochemicalValue;
}

void UCognitiveLoad3::AccumulateCognitiveLoad(float CurrentTime)
{
    const float Delta = CurrentTime - PreviousTime;
    if (Delta <= KINDA_SMALL_NUMBER)
    {
        return;
    }

    const float CurrentInteraction = CalculateInteractionFunction(CurrentTime);
    const float PreviousInteraction = CalculateInteractionFunction(PreviousTime);
    CognitiveLoadValue += 0.5f * (PreviousInteraction + CurrentInteraction) * Delta;
}
