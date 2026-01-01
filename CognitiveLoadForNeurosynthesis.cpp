#include "CognitiveLoadForNeurosynthesis.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoadForNeurosynthesis);

UCognitiveLoadForNeurosynthesis::UCognitiveLoadForNeurosynthesis()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveLoadForNeurosynthesis::BeginPlay()
{
    Super::BeginPlay();

    CognitiveLoadValue = CalculateCognitiveLoad();
    UE_LOG(LogCognitiveLoadForNeurosynthesis, Log, TEXT("Cognitive Load (L_n): %f"), CognitiveLoadValue);
}

float UCognitiveLoadForNeurosynthesis::InteractionFunction(float Nc, float Sb) const
{
    return Nc + Sb;
}

float UCognitiveLoadForNeurosynthesis::CalculateCognitiveLoad() const
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
        UE_LOG(LogCognitiveLoadForNeurosynthesis, Warning, TEXT("Time constant must be greater than zero."));
        return 0.f;
    }

    return Integral / TimeConstant;
}

void UCognitiveLoadForNeurosynthesis::SetTimeConstant(float Value)
{
    TimeConstant = Value;
}

void UCognitiveLoadForNeurosynthesis::SetStartTime(float Value)
{
    StartTime = Value;
}

void UCognitiveLoadForNeurosynthesis::SetEndTime(float Value)
{
    EndTime = Value;
}

void UCognitiveLoadForNeurosynthesis::SetNumSamples(int32 Value)
{
    NumSamples = Value;
}
