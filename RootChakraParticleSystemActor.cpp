#include "RootChakraParticleSystemActor.h"

ARootChakraParticleSystemActor::ARootChakraParticleSystemActor()
{
    // Red, fully opaque per Unity script
    AuraColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
    AuraTransparency = 1.0f;

    // Small particles, moderate count
    ParticleSize = 0.05f;
    ParticleCount = 1000;

    // Dense small sphere radius: 0.2 Unity units ≈ 20 cm
    AuraRadius = 20.f;
}

