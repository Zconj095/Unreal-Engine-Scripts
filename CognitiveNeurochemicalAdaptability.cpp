#include "CognitiveNeurochemicalAdaptability.h"

DEFINE_LOG_CATEGORY(LogCognitiveNeurochemicalAdaptability);

UCognitiveNeurochemicalAdaptability::UCognitiveNeurochemicalAdaptability()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveNeurochemicalAdaptability::BeginPlay()
{
    Super::BeginPlay();

    CNAValue = CalculateCNA();
    UE_LOG(LogCognitiveNeurochemicalAdaptability, Log, TEXT("Cognitive Neurochemical Adaptability (CNA): %f"), CNAValue);
}

float UCognitiveNeurochemicalAdaptability::CalculateCNA() const
{
    if (FMath::IsNearlyZero(DeltaTime))
    {
        UE_LOG(LogCognitiveNeurochemicalAdaptability, Error, TEXT("Time interval (Δt) cannot be zero."));
        return 0.f;
    }

    return Theta * (DeltaNAdapt / DeltaTime);
}
