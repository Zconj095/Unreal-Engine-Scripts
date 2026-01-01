#include "CognitiveLoadEnergetics.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoadEnergetics);

UCognitiveLoadEnergetics::UCognitiveLoadEnergetics()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveLoadEnergetics::BeginPlay()
{
    Super::BeginPlay();
}

void UCognitiveLoadEnergetics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    ElapsedTime += DeltaTime;
    const float InteractionValue = InteractionFunction(EnvironmentalEnergy, InternalEnergy);

    if (ElapsedTime >= IntegrationTimeStep)
    {
        const float PreviousInteraction = InteractionFunction(EnvironmentalEnergy, InternalEnergy);
        AccumulatedIntegral += 0.5f * (PreviousInteraction + InteractionValue) * IntegrationTimeStep;
        ElapsedTime = 0.f;
    }

    if (ProcessingTimeConstant > KINDA_SMALL_NUMBER)
    {
        CognitiveLoadValue = AccumulatedIntegral / ProcessingTimeConstant;
    }
    else
    {
        CognitiveLoadValue = 0.f;
        UE_LOG(LogCognitiveLoadEnergetics, Warning, TEXT("ProcessingTimeConstant must be > 0"));
    }

    UE_LOG(LogCognitiveLoadEnergetics, Verbose, TEXT("Environmental Energy (E_t): %.2f"), EnvironmentalEnergy);
    UE_LOG(LogCognitiveLoadEnergetics, Verbose, TEXT("Internal Energy (E_i): %.2f"), InternalEnergy);
    UE_LOG(LogCognitiveLoadEnergetics, Verbose, TEXT("Cognitive Load (L_e): %.2f"), CognitiveLoadValue);
}

float UCognitiveLoadEnergetics::InteractionFunction(float Et, float Ei) const
{
    return Et * Ei;
}
