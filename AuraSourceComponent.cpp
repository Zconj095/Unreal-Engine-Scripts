#include "AuraSourceComponent.h"

#include "Math/UnrealMathUtility.h"

UAuraSourceComponent::UAuraSourceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAuraSourceComponent::BeginPlay()
{
    Super::BeginPlay();

    // Unity Start(): initialize signal params
    Signal.Add(TEXT("freq"), static_cast<float>(FMath::RandRange(1, 11))); // Unity int Range(1,12) => [1,11]
    Signal.Add(TEXT("phase"), FMath::FRand());
    Signal.Add(TEXT("amplitude"), FMath::FRand());

    // Generate initial signal
    RawSignal = GenerateSignal();
}

TArray<float> UAuraSourceComponent::GenerateSignal()
{
    TArray<float> Output;
    const int32 N = FMath::Max(0, SampleCount);
    Output.SetNum(N);

    const float* AmpPtr = Signal.Find(TEXT("amplitude"));
    const float* FreqPtr = Signal.Find(TEXT("freq"));
    const float* PhasePtr = Signal.Find(TEXT("phase"));

    const float Amplitude = AmpPtr ? *AmpPtr : 0.f;
    const float Freq = FreqPtr ? *FreqPtr : 0.f;
    const float Phase = PhasePtr ? *PhasePtr : 0.f;

    for (int32 i = 0; i < N; ++i)
    {
        const float Value = Amplitude * FMath::Sin((Freq / 1.3f) * Phase + FMath::FRand() * PI);
        Output[i] = Value;
    }

    RawSignal = Output;
    return Output;
}

