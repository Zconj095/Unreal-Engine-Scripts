#include "AstralBodyParticleSystemActor.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AAstralBodyParticleSystemActor::AAstralBodyParticleSystemActor()
{
    PrimaryActorTick.bCanEverTick = false;

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    SetRootComponent(NiagaraComponent);
    if (NiagaraComponent)
    {
        NiagaraComponent->bAutoActivate = false;
    }
}

void AAstralBodyParticleSystemActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    ApplyParameters();
}

void AAstralBodyParticleSystemActor::BeginPlay()
{
    Super::BeginPlay();
    ApplyParameters();
}

void AAstralBodyParticleSystemActor::ApplyParameters()
{
    if (!NiagaraComponent)
    {
        return;
    }

    if (AuraSystem)
    {
        NiagaraComponent->SetAsset(AuraSystem);
    }

    // Compose color with alpha from transparency
    const FLinearColor ColorWithAlpha(AuraColor.R, AuraColor.G, AuraColor.B, FMath::Clamp(AuraTransparency, 0.f, 1.f));

    // These names must match User parameters in the Niagara System
    NiagaraComponent->SetVariableLinearColor(TEXT("User.AuraColor"), ColorWithAlpha);
    NiagaraComponent->SetVariableFloat(TEXT("User.ParticleSize"), ParticleSize);
    NiagaraComponent->SetVariableInt(TEXT("User.SpawnCount"), ParticleCount);
    NiagaraComponent->SetVariableFloat(TEXT("User.Radius"), AuraRadius);

    if (!NiagaraComponent->IsActive())
    {
        NiagaraComponent->Activate(true);
    }
}

