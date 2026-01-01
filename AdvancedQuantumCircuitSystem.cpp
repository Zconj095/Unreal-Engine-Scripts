// Copyright Epic Games, Inc.

#include "AdvancedQuantumCircuitSystem.h"
#include "Components/SceneComponent.h"

AAdvancedQuantumCircuitSystem::AAdvancedQuantumCircuitSystem()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    CircuitExecutor = CreateDefaultSubobject<UDynamicCircuitExecutorComponent>(TEXT("CircuitExecutor"));
    CircuitInteraction = CreateDefaultSubobject<UQuantumCircuitInteractionComponent>(TEXT("CircuitInteraction"));
}

void AAdvancedQuantumCircuitSystem::BeginPlay()
{
    Super::BeginPlay();

    if (CircuitExecutor)
    {
        CircuitExecutor->ExecuteCircuitWithVisualization();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AdvancedQuantumCircuitSystem missing CircuitExecutor component."));
    }
}
