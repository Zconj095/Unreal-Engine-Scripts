#include "SeasonalNighttimePatterns.h"

ASeasonalNighttimePatterns::ASeasonalNighttimePatterns()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASeasonalNighttimePatterns::BeginPlay()
{
    Super::BeginPlay();
    // Data container only; no runtime behavior.
}

