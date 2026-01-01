#include "HopfieldNetworkIntegration.h"

FHopfieldNetworkIntegration::FHopfieldNetworkIntegration(int32 InDimensions)
    : Dimensions(FMath::Max(0, InDimensions))
{
    Weights.Init(0.f, Dimensions * Dimensions);
}

void FHopfieldNetworkIntegration::Train(const TArray<float>& Pattern)
{
    if (Dimensions <= 0)
    {
        return;
    }
    const int32 N = FMath::Min(Dimensions, Pattern.Num());
    for (int32 i = 0; i < N; ++i)
    {
        const float Pi = Pattern[i];
        for (int32 j = 0; j < N; ++j)
        {
            if (i == j) continue;
            Weights[Index(i, j)] += Pi * Pattern[j];
        }
    }
}

TArray<float> FHopfieldNetworkIntegration::Recall(const TArray<float>& Input) const
{
    TArray<float> Output;
    Output.Init(0.f, Dimensions);

    if (Dimensions <= 0 || Input.Num() <= 0)
    {
        return Output;
    }

    const int32 N = FMath::Min(Dimensions, Input.Num());
    for (int32 i = 0; i < Dimensions; ++i)
    {
        float Sum = 0.f;
        for (int32 j = 0; j < N; ++j)
        {
            Sum += Weights[Index(i, j)] * Input[j];
        }
        Output[i] = (Sum >= 0.f) ? 1.f : -1.f;
    }
    return Output;
}

