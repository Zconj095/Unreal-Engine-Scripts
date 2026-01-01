#include "CognitiveLoadParticlePhysics.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoadParticlePhysics);

UCognitiveLoadParticlePhysics::UCognitiveLoadParticlePhysics()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveLoadParticlePhysics::BeginPlay()
{
    Super::BeginPlay();

    CognitiveLoadValue = CalculateCognitiveLoad();
    UE_LOG(LogCognitiveLoadParticlePhysics, Log, TEXT("Cognitive Load for Particle Physics (Lp): %f"), CognitiveLoadValue);
}

float UCognitiveLoadParticlePhysics::CalculateCognitiveLoad() const
{
    const int32 Steps = FMath::Max(1, IntegrationSteps);
    const float Duration = EndTime - StartTime;
    const float Dt = Duration / static_cast<float>(Steps);

    float Sum = 0.f;
    for (int32 Index = 0; Index < Steps; ++Index)
    {
        const float CurrentTime = StartTime + Index * Dt;
        const float PhiValue = InteractionFunction(CurrentTime);
        Sum += PhiValue * Dt;
    }

    if (Tau <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCognitiveLoadParticlePhysics, Warning, TEXT("Tau must be greater than zero when computing cognitive load."));
        return 0.f;
    }

    return Sum / Tau;
}

float UCognitiveLoadParticlePhysics::InteractionFunction(float Time) const
{
    const float Eq = FMath::Sin(Time);
    const float Pq = FMath::Cos(Time);
    return Eq * Pq;
}
