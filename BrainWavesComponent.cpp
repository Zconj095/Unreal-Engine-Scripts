#include "BrainWavesComponent.h"

#include "Math/UnrealMathUtility.h"

UBrainWavesComponent::UBrainWavesComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBrainWavesComponent::BeginPlay()
{
    Super::BeginPlay();
    GenerateWaves();
}

void UBrainWavesComponent::GenerateWaves()
{
    const int32 N = FMath::Max(0, NumSamples);
    Ts.SetNum(N);
    if (N > 1)
    {
        const float Step = DurationSeconds / static_cast<float>(N - 1);
        for (int32 i = 0; i < N; ++i)
        {
            Ts[i] = static_cast<float>(i) * Step;
        }
    }
    else if (N == 1)
    {
        Ts[0] = 0.f;
    }

    Waves.Empty();
    {
        FFloatArray Arr; Arr.Values = MakeWave(2.0f, 1.5f);
        Waves.Add(TEXT("delta"), MoveTemp(Arr));
    }
    {
        FFloatArray Arr; Arr.Values = MakeWave(5.0f, 2.0f);
        Waves.Add(TEXT("theta"), MoveTemp(Arr));
    }
    {
        FFloatArray Arr; Arr.Values = MakeWave(11.0f, 2.5f);
        Waves.Add(TEXT("alpha"), MoveTemp(Arr));
    }
}

TArray<float> UBrainWavesComponent::MakeWave(float Freq, float Amplitude) const
{
    TArray<float> Result;
    Result.SetNum(Ts.Num());
    for (int32 i = 0; i < Ts.Num(); ++i)
    {
        Result[i] = Amplitude * FMath::Sin(Freq * Ts[i]);
    }
    return Result;
}
