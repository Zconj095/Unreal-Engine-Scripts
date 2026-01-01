#include "EmotionalBodyParticleSystemActor.h"

AEmotionalBodyParticleSystemActor::AEmotionalBodyParticleSystemActor()
{
    AuraColor = FLinearColor(1.0f, 0.7f, 0.7f, 1.0f); // Light red/pink
    AuraTransparency = 0.3f;
    ParticleSize = 0.1f;
    ParticleCount = 2000;
    AuraRadius = 200.f; // 2.0 Unity units ≈ 200 cm
}

