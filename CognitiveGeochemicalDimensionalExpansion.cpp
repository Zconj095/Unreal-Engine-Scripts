#include "CognitiveGeochemicalDimensionalExpansion.h"

DEFINE_LOG_CATEGORY(LogCognitiveGeochemicalDimensionalExpansion);

UCognitiveGeochemicalDimensionalExpansion::UCognitiveGeochemicalDimensionalExpansion()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveGeochemicalDimensionalExpansion::BeginPlay()
{
    Super::BeginPlay();

    CGDE = CalculateCGDE();
    UE_LOG(LogCognitiveGeochemicalDimensionalExpansion, Log, TEXT("Cognitive Geochemical Dimensional Expansion (CGDE): %f"), CGDE);
}

float UCognitiveGeochemicalDimensionalExpansion::CalculateCGDE() const
{
    if (TotalNeurons <= 0.f)
    {
        UE_LOG(LogCognitiveGeochemicalDimensionalExpansion, Error, TEXT("Total neurons (N) must be greater than zero."));
        return 0.f;
    }

    const float LogTerm = FMath::Log2(1.f + ConnectivityDensity);
    return Theta * FMath::Pow(TotalNeurons, Dimensionality) * LogTerm;
}
