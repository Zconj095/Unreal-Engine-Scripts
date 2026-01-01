#include "SensorArrayComponent.h"

#include "Math/UnrealMathUtility.h"

USensorArrayComponent::USensorArrayComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USensorArrayComponent::Measure()
{
    // EEG: 20 random values in [0,1)
    FFloatArray EEGSample; EEGSample.Values.SetNum(20);
    for (int32 i = 0; i < 20; ++i)
    {
        EEGSample.Values[i] = FMath::FRand();
    }
    EEG.Add(MoveTemp(EEGSample));

    // EKG: 5 random values in [0,1) + 10
    FFloatArray EKGSample; EKGSample.Values.SetNum(5);
    for (int32 i = 0; i < 5; ++i)
    {
        EKGSample.Values[i] = FMath::FRand() + 10.0f;
    }
    EKG.Add(MoveTemp(EKGSample));
}

float USensorArrayComponent::GetBpm() const
{
    if (EKG.Num() == 0)
    {
        return 0.f;
    }

    const TArray<float>& Last = EKG.Last().Values;
    int32 Count = 0;
    for (float V : Last)
    {
        if (V > 10.5f) { ++Count; }
    }
    return static_cast<float>(Count) / 4.0f;
}
