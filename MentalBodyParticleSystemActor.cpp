#include "MentalBodyParticleSystemActor.h"

AMentalBodyParticleSystemActor::AMentalBodyParticleSystemActor()
{
    AuraColor = FLinearColor(1.0f, 1.0f, 0.6f, 1.0f); // Light yellow
    AuraTransparency = 0.3f;
    ParticleSize = 0.1f;
    ParticleCount = 2000;
    AuraRadius = 250.f; // 2.5 Unity units ≈ 250 cm
}

