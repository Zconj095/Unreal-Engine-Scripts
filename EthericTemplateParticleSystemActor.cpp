#include "EthericTemplateParticleSystemActor.h"

AEthericTemplateParticleSystemActor::AEthericTemplateParticleSystemActor()
{
    AuraColor = FLinearColor(0.7f, 0.7f, 1.0f, 1.0f); // Light purple/blue
    AuraTransparency = 0.3f;
    ParticleSize = 0.1f;
    ParticleCount = 2000;
    AuraRadius = 350.f; // 3.5 Unity units ≈ 350 cm
}

