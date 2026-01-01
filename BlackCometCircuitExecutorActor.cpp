// Copyright Epic Games, Inc.

#include "BlackCometCircuitExecutorActor.h"

#include "ChaosStateCircuitComponent.h"
#include "EntangledChaosCircuitComponent.h"
#include "EntangledStateCircuitComponent.h"
#include "ParticleStateCircuitComponent.h"
#include "SuperpositionedImposedFieldCircuitComponent.h"
#include "Components/SceneComponent.h"

ABlackCometCircuitExecutorActor::ABlackCometCircuitExecutorActor()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    EntangledStateCircuit = CreateDefaultSubobject<UEntangledStateCircuitComponent>(TEXT("EntangledStateCircuit"));
    SuperpositionedFieldCircuit = CreateDefaultSubobject<USuperpositionedImposedFieldCircuitComponent>(TEXT("SuperpositionedFieldCircuit"));
    EntangledChaosCircuit = CreateDefaultSubobject<UEntangledChaosCircuitComponent>(TEXT("EntangledChaosCircuit"));
    ChaosStateCircuit = CreateDefaultSubobject<UChaosStateCircuitComponent>(TEXT("ChaosStateCircuit"));
    ParticleStateCircuit = CreateDefaultSubobject<UParticleStateCircuitComponent>(TEXT("ParticleStateCircuit"));
}

void ABlackCometCircuitExecutorActor::BeginPlay()
{
    Super::BeginPlay();

    if (EntangledStateCircuit)
    {
        EntangledStateCircuit->CreateEntangledState(NumQubits);
    }

    if (SuperpositionedFieldCircuit)
    {
        SuperpositionedFieldCircuit->ApplyFieldToSuperposition(NumQubits, ImposedField);
    }

    if (EntangledChaosCircuit)
    {
        EntangledChaosCircuit->CreateEntangledChaos(NumQubits);
    }

    if (ChaosStateCircuit)
    {
        ChaosStateCircuit->CreateChaosState(NumQubits);
    }

    if (ParticleStateCircuit)
    {
        ParticleStateCircuit->CreateParticleState(NumQubits, InitialPosition);
    }
}
