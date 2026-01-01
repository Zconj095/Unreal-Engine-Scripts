#include "DevelopmentAdaptabilityRatio.h"

DEFINE_LOG_CATEGORY(LogDevelopmentAdaptabilityRatio);

UDevelopmentAdaptabilityRatio::UDevelopmentAdaptabilityRatio()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UDevelopmentAdaptabilityRatio::BeginPlay()
{
    Super::BeginPlay();
    UpdateDAR();
}

void UDevelopmentAdaptabilityRatio::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateDAR();
    UE_LOG(LogDevelopmentAdaptabilityRatio, Verbose, TEXT("DAR: %.6f"), DARValue);
}

void UDevelopmentAdaptabilityRatio::UpdateDAR()
{
    if (DeltaT <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogDevelopmentAdaptabilityRatio, Error, TEXT("Time interval (ΔT) must be greater than zero."));
        DARValue = 0.f;
        return;
    }

    DARValue = (DeltaP / DeltaT) * Eta;
}
