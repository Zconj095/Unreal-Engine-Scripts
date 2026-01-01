#include "CausalBodyParticleSystemActor.h"

ACausalBodyParticleSystemActor::ACausalBodyParticleSystemActor()
{
    AuraColor = FLinearColor(1.f, 1.f, 1.f, 1.f); // White
    AuraTransparency = 0.3f;
    ParticleSize = 0.1f;
    ParticleCount = 2000;
    AuraRadius = 450.f; // 4.5 Unity units ≈ 450 cm
}

