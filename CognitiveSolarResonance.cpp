#include "CognitiveSolarResonance.h"

DEFINE_LOG_CATEGORY(LogCognitiveSolarResonance);

UCognitiveSolarResonance::UCognitiveSolarResonance()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveSolarResonance::BeginPlay()
{
    Super::BeginPlay();
    TimeElapsed = 0.f;
}

void UCognitiveSolarResonance::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeElapsed += DeltaTime;
    const float CognitiveFrequency = CalculateFrequency(TimeElapsed);
    UE_LOG(LogCognitiveSolarResonance, Verbose, TEXT("Time: %.2fs, Cognitive Frequency: %.4f"), TimeElapsed, CognitiveFrequency);
}

float UCognitiveSolarResonance::CalculateFrequency(float TimeSeconds) const
{
    return F0 + DeltaF * FMath::Cos(OmegaS * TimeSeconds + Phi);
}
