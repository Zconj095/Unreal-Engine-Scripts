#include "CognitiveLoad.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoad);

UCognitiveLoad::UCognitiveLoad()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveLoad::BeginPlay()
{
    Super::BeginPlay();
    PreviousTime = GetWorld()->GetTimeSeconds();
}

void UCognitiveLoad::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float CurrentTime = GetWorld()->GetTimeSeconds();
    const float Delta = CurrentTime - PreviousTime;
    if (Delta > KINDA_SMALL_NUMBER && Tau > KINDA_SMALL_NUMBER)
    {
        const float Interaction = InteractionFunction(StimuliFromCelestialPatterns, LuminosityStimuli);
        CognitiveLoadValue += Interaction * Delta / Tau;
    }
    PreviousTime = CurrentTime;

    UE_LOG(LogCognitiveLoad, Verbose, TEXT("Cognitive Load (L_a): %f"), CognitiveLoadValue);
}

float UCognitiveLoad::InteractionFunction(float Sc, float Lo) const
{
    return Sc * Lo;
}

void UCognitiveLoad::UpdateStimuli(float NewSc, float NewLo)
{
    StimuliFromCelestialPatterns = NewSc;
    LuminosityStimuli = NewLo;
}

float UCognitiveLoad::GetCognitiveLoad() const
{
    return CognitiveLoadValue;
}
