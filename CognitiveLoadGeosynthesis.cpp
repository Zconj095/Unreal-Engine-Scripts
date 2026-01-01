#include "CognitiveLoadGeosynthesis.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoadGeosynthesis);

UCognitiveLoadGeosynthesis::UCognitiveLoadGeosynthesis()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveLoadGeosynthesis::BeginPlay()
{
    Super::BeginPlay();
    ElapsedTime = 0.f;
    IntegralSum = 0.f;
    TotalTime = 0.f;
}

void UCognitiveLoadGeosynthesis::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TotalTime += DeltaTime;
    ElapsedTime += DeltaTime;
    const float InteractionValue = InteractionFunction(GeologicalStimuli, GeomorphicStimuli);

    if (ElapsedTime >= TimeStep && TimeStep > KINDA_SMALL_NUMBER)
    {
        IntegralSum += InteractionValue * TimeStep;
        ElapsedTime -= TimeStep;
    }

    if (ProcessingTimeConstant > KINDA_SMALL_NUMBER)
    {
        CognitiveLoadValue = IntegralSum / ProcessingTimeConstant;
    }
    else
    {
        CognitiveLoadValue = 0.f;
        UE_LOG(LogCognitiveLoadGeosynthesis, Warning, TEXT("ProcessingTimeConstant must be greater than zero."));
    }

    UE_LOG(LogCognitiveLoadGeosynthesis, Verbose, TEXT("Time: %.2f Interaction: %.2f Cognitive Load: %.2f"), TotalTime, InteractionValue, CognitiveLoadValue);
}

float UCognitiveLoadGeosynthesis::InteractionFunction(float Sg, float Gm) const
{
    return Sg * Gm;
}
