#include "CognitiveGeologicalAdaptability.h"

DEFINE_LOG_CATEGORY(LogCognitiveGeologicalAdaptability);

UCognitiveGeologicalAdaptability::UCognitiveGeologicalAdaptability()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveGeologicalAdaptability::BeginPlay()
{
    Super::BeginPlay();

    UpdateAdaptability();
}

void UCognitiveGeologicalAdaptability::TickComponent(float InDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(InDeltaTime, TickType, ThisTickFunction);

    UpdateAdaptability();
}

void UCognitiveGeologicalAdaptability::UpdateAdaptability()
{
    if (DeltaTime <= 0.f)
    {
        UE_LOG(LogCognitiveGeologicalAdaptability, Error, TEXT("Time period (?t) must be greater than zero."));
        CGA = 0.f;
        return;
    }

    CGA = Alpha * (DeltaNGeo / DeltaTime);
}

void UCognitiveGeologicalAdaptability::SetAlpha(float NewAlpha)
{
    Alpha = NewAlpha;
}

void UCognitiveGeologicalAdaptability::SetDeltaNGeo(float NewDeltaNGeo)
{
    DeltaNGeo = NewDeltaNGeo;
}

void UCognitiveGeologicalAdaptability::SetDeltaTime(float NewDeltaTime)
{
    if (NewDeltaTime > 0.f)
    {
        DeltaTime = NewDeltaTime;
    }
    else
    {
        UE_LOG(LogCognitiveGeologicalAdaptability, Error, TEXT("Time period (?t) must be greater than zero."));
    }
}

float UCognitiveGeologicalAdaptability::GetCGA() const
{
    return CGA;
}
