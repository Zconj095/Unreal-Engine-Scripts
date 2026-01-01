#include "CognitiveFocusCalculator.h"

UCognitiveFocusCalculator::UCognitiveFocusCalculator()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveFocusCalculator::BeginPlay()
{
    Super::BeginPlay();

    RecalculateFocus();
}

void UCognitiveFocusCalculator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    RecalculateFocus();
}

void UCognitiveFocusCalculator::RecalculateFocus()
{
    CognitiveFocus = CognitiveEfficiencyScaling * (MoonIllumination + GravitationalWeightFactor * MoonGravitationalPull);
}
