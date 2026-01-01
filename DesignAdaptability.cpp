#include "DesignAdaptability.h"

DEFINE_LOG_CATEGORY(LogDesignAdaptability);

UDesignAdaptability::UDesignAdaptability()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDesignAdaptability::BeginPlay()
{
    Super::BeginPlay();

    AdaptabilityIndex = CalculateAdaptability();
    UE_LOG(LogDesignAdaptability, Log, TEXT("Adaptability Index (A): %.6f"), AdaptabilityIndex);
}

float UDesignAdaptability::CalculateAdaptability() const
{
    if (FMath::IsNearlyZero(DeltaEnvironment))
    {
        UE_LOG(LogDesignAdaptability, Warning, TEXT("DeltaEnvironment must be non-zero."));
        return 0.f;
    }

    return DeltaDesign / DeltaEnvironment;
}
