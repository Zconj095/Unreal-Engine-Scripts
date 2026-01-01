#include "CrossCorrelation1DComponent.h"

#include "Logging/LogMacros.h"

UCrossCorrelation1DComponent::UCrossCorrelation1DComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCrossCorrelation1DComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Signal1.Num() == 0 || Signal2.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CrossCorrelation1DComponent: Signals must not be empty."));
        return;
    }

    const TArray<float> Result = ComputeCrossCorrelation1D(Signal1, Signal2);

    UE_LOG(LogTemp, Log, TEXT("Cross-Correlation Result:"));
    for (int32 i = 0; i < Result.Num(); ++i)
    {
        const int32 Lag = i - (Signal2.Num() - 1);
        UE_LOG(LogTemp, Log, TEXT("Lag %d: %f"), Lag, Result[i]);
    }
}

TArray<float> UCrossCorrelation1DComponent::ComputeCrossCorrelation1D(const TArray<float>& InSignal1, const TArray<float>& InSignal2) const
{
    TArray<float> Correlation;

    const int32 Len1 = InSignal1.Num();
    const int32 Len2 = InSignal2.Num();
    if (Len1 == 0 || Len2 == 0)
    {
        return Correlation;
    }

    const int32 Length = Len1 + Len2 - 1;
    Correlation.Init(0.f, Length);

    const TArray<float> NormalizedSignal2 = NormalizeSignal(InSignal2);

    for (int32 i = 0; i < Length; ++i)
    {
        float Accum = 0.f;
        for (int32 j = 0; j < Len1; ++j)
        {
            const int32 Signal2Index = i - j;
            if (Signal2Index >= 0 && Signal2Index < Len2)
            {
                Accum += InSignal1[j] * NormalizedSignal2[Signal2Index];
            }
        }
        Correlation[i] = Accum;
    }

    return Correlation;
}

TArray<float> UCrossCorrelation1DComponent::NormalizeSignal(const TArray<float>& Signal) const
{
    TArray<float> Normalized;
    const int32 N = Signal.Num();
    if (N == 0)
    {
        return Normalized;
    }

    double Sum = 0.0;
    for (float V : Signal)
    {
        Sum += V;
    }
    const double Mean = Sum / N;

    Normalized.SetNumUninitialized(N);
    for (int32 i = 0; i < N; ++i)
    {
        Normalized[i] = Signal[i] - static_cast<float>(Mean);
    }
    return Normalized;
}
