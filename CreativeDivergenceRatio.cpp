#include "CreativeDivergenceRatio.h"

DEFINE_LOG_CATEGORY(LogCreativeDivergenceRatio);

UCreativeDivergenceRatio::UCreativeDivergenceRatio()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCreativeDivergenceRatio::BeginPlay()
{
    Super::BeginPlay();
    CalculateCDR();
    UE_LOG(LogCreativeDivergenceRatio, Log, TEXT("Creative Divergence Ratio (CDR): %.6f"), CDRValue);
}

void UCreativeDivergenceRatio::CalculateCDR()
{
    if (Omega <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCreativeDivergenceRatio, Error, TEXT("Omega (Ω) must be greater than zero."));
        CDRValue = 0.f;
        return;
    }

    CDRValue = (Phi * DeltaTheta) / Omega;
}
