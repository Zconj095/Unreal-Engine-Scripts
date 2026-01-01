#include "CognitiveLoad4.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoad4);

UCognitiveLoad4::UCognitiveLoad4()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveLoad4::BeginPlay()
{
    Super::BeginPlay();
    PhiHistory.Reset();
}

void UCognitiveLoad4::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float CurrentPhi = InteractionFunction(EnvironmentalStimuli, MolecularStimuli);
    PhiHistory.Add(CurrentPhi);

    constexpr int32 MaxHistory = 512;
    if (PhiHistory.Num() > MaxHistory)
    {
        PhiHistory.RemoveAt(0, PhiHistory.Num() - MaxHistory, EAllowShrinking::No);
    }

    const float IntegratedValue = NumericalIntegration();
    if (Tau > KINDA_SMALL_NUMBER)
    {
        CognitiveLoadValue = IntegratedValue / Tau;
    }
    else
    {
        CognitiveLoadValue = 0.f;
        UE_LOG(LogCognitiveLoad4, Warning, TEXT("Tau must be greater than zero."));
    }

    UE_LOG(LogCognitiveLoad4, Verbose, TEXT("Cognitive Load (Lb): %.2f"), CognitiveLoadValue);
}

float UCognitiveLoad4::InteractionFunction(float Environment, float Molecular) const
{
    return Environment * Molecular;
}

float UCognitiveLoad4::NumericalIntegration() const
{
    if (PhiHistory.Num() < 2 || IntegrationInterval <= KINDA_SMALL_NUMBER)
    {
        return 0.f;
    }

    float Result = 0.f;
    for (int32 Index = 1; Index < PhiHistory.Num(); ++Index)
    {
        const float PhiAverage = (PhiHistory[Index - 1] + PhiHistory[Index]) * 0.5f;
        Result += PhiAverage * IntegrationInterval;
    }

    return Result;
}
