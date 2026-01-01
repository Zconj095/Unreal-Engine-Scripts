#include "BrainActivityProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

UBrainActivityProcessorComponent::UBrainActivityProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBrainActivityProcessorComponent::EncodingDifferences(const TArray<float>& Activity, TArray<float>& Differences) const
{
    const int32 N = Activity.Num();
    Differences.SetNum(FMath::Max(0, N - 1));
    for (int32 i = 0; i + 1 < N; ++i)
    {
        Differences[i] = Activity[i + 1] - Activity[i];
    }
}

void UBrainActivityProcessorComponent::LowFrequencyFluctuations(TArray<float>& OutFluctuations) const
{
    OutFluctuations.SetNum(100);
    for (int32 i = 0; i < 100; ++i)
    {
        // i * 2 * PI / 99
        const float Arg = static_cast<float>(i) * 2.0f * PI / 99.0f;
        OutFluctuations[i] = FMath::Sin(Arg);
    }
}

void UBrainActivityProcessorComponent::FunctionalConnectivity(const TArray<float>& Activity, TArray<FFloatArray>& OutConnectivity) const
{
    const int32 Len = Activity.Num();
    OutConnectivity.SetNum(Len);
    for (int32 i = 0; i < Len; ++i)
    {
        FFloatArray& Row = OutConnectivity[i];
        Row.Values.SetNum(Len);
        for (int32 j = 0; j < Len; ++j)
        {
            Row.Values[j] = (Activity[i] + Activity[j]) * 0.5f;
        }
    }
}

void UBrainActivityProcessorComponent::GenerateLinearSpace(float Start, float End, int32 Count, TArray<float>& OutArray) const
{
    if (Count <= 0)
    {
        OutArray.Reset();
        return;
    }

    OutArray.SetNum(Count);
    if (Count == 1)
    {
        OutArray[0] = Start;
        return;
    }

    const float Step = (End - Start) / static_cast<float>(Count - 1);
    for (int32 i = 0; i < Count; ++i)
    {
        OutArray[i] = Start + Step * static_cast<float>(i);
    }
}
