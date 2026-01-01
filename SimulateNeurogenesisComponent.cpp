#include "SimulateNeurogenesisComponent.h"

#include "Math/UnrealMathUtility.h"

USimulateNeurogenesisComponent::USimulateNeurogenesisComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USimulateNeurogenesisComponent::Simulate(float Dt, float Duration, TArray<float>& OutTimes, TArray<FFloatArray>& OutStates) const
{
    OutTimes.Reset();
    OutStates.Reset();

    if (Dt <= 0.f || Duration < 0.f)
    {
        return;
    }

    const int32 Steps = FMath::FloorToInt(Duration / Dt) + 1;

    OutTimes.SetNum(Steps);
    OutStates.SetNum(Steps);

    for (int32 i = 0; i < Steps; ++i)
    {
        OutTimes[i] = static_cast<float>(i) * Dt;
        FFloatArray& State = OutStates[i];
        State.Values.Init(0.f, 3); // Placeholder for G, Z, D
    }
}
