#include "EthericBodyParticleSystemActor.h"

AEthericBodyParticleSystemActor::AEthericBodyParticleSystemActor()
{
    AuraColor = FLinearColor(0.9f, 0.9f, 1.0f, 1.0f); // Light blue
    AuraTransparency = 0.3f;
    ParticleSize = 0.1f;
    ParticleCount = 2000;
    AuraRadius = 150.f; // 1.5 Unity units ≈ 150 cm
}

