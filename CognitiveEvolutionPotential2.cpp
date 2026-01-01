#include "CognitiveEvolutionPotential2.h"

DEFINE_LOG_CATEGORY(LogCognitiveEvolutionPotential2);

UCognitiveEvolutionPotential2::UCognitiveEvolutionPotential2()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveEvolutionPotential2::BeginPlay()
{
    Super::BeginPlay();

    CepValue = CalculateCEP();
    UE_LOG(LogCognitiveEvolutionPotential2, Log, TEXT("Cognitive Evolution Potential (CEP): %f"), CepValue);
}

float UCognitiveEvolutionPotential2::CalculateCEP() const
{
    const int32 Steps = FMath::Max(1, NumSteps);
    const float Dx = (XMax - XMin) / static_cast<float>(Steps);
    float Sum = 0.f;

    for (int32 Index = 0; Index < Steps; ++Index)
    {
        const float X = XMin + Index * Dx;
        const float WaveValue = WaveFunction(X, T);
        const float ConjugateValue = ConjugateWaveFunction(X, T);
        Sum += ConjugateValue * WaveValue * Dx;
    }

    return Sum;
}

float UCognitiveEvolutionPotential2::WaveFunction(float X, float Time) const
{
    return FMath::Sin(X + Time);
}

float UCognitiveEvolutionPotential2::ConjugateWaveFunction(float X, float Time) const
{
    return FMath::Sin(X + Time);
}
