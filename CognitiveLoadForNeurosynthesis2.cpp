#include "CognitiveLoadForNeurosynthesis2.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoadForNeurosynthesis2);

UCognitiveLoadForNeurosynthesis2::UCognitiveLoadForNeurosynthesis2()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveLoadForNeurosynthesis2::BeginPlay()
{
    Super::BeginPlay();

    CognitiveLoadValue = CalculateCognitiveLoad();
    UE_LOG(LogCognitiveLoadForNeurosynthesis2, Log, TEXT("Cognitive Load (L_n): %f"), CognitiveLoadValue);
}

float UCognitiveLoadForNeurosynthesis2::InteractionFunction(float Nc, float Sb) const
{
    return Nc + Sb;
}

float UCognitiveLoadForNeurosynthesis2::CalculateCognitiveLoad() const
{
    const int32 Samples = FMath::Max(1, NumSamples);
    const float Dt = (EndTime - StartTime) / static_cast<float>(Samples);

    float Integral = 0.f;
    for (int32 Index = 0; Index < Samples; ++Index)
    {
        const float CurrentTime = StartTime + Index * Dt;
        const float Nc = FMath::Sin(CurrentTime);
        const float Sb = FMath::Cos(CurrentTime);
        const float Phi = InteractionFunction(Nc, Sb);
        Integral += Phi * Dt;
    }

    if (TimeConstant <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCognitiveLoadForNeurosynthesis2, Warning, TEXT("Time constant must be greater than zero."));
        return 0.f;
    }

    return Integral / TimeConstant;
}

void UCognitiveLoadForNeurosynthesis2::SetTimeConstant(float Value)
{
    TimeConstant = Value;
}

void UCognitiveLoadForNeurosynthesis2::SetStartTime(float Value)
{
    StartTime = Value;
}

void UCognitiveLoadForNeurosynthesis2::SetEndTime(float Value)
{
    EndTime = Value;
}

void UCognitiveLoadForNeurosynthesis2::SetNumSamples(int32 Value)
{
    NumSamples = Value;
}
