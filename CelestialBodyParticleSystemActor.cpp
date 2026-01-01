#include "CelestialBodyParticleSystemActor.h"

ACelestialBodyParticleSystemActor::ACelestialBodyParticleSystemActor()
{
    AuraColor = FLinearColor(0.9f, 0.6f, 1.0f, 1.0f); // Light purple
    AuraTransparency = 0.3f;
    ParticleSize = 0.1f;
    ParticleCount = 2000;
    AuraRadius = 400.f; // 4.0 Unity units ≈ 400 cm
}

