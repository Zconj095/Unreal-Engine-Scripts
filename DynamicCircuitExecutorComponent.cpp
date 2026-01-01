// Copyright Epic Games, Inc.

#include "DynamicCircuitExecutorComponent.h"

#include "QuantumCircuitVisualizerComponent.h"
#include "SuperpositionAnimatorComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UDynamicCircuitExecutorComponent::UDynamicCircuitExecutorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDynamicCircuitExecutorComponent::BeginPlay()
{
    Super::BeginPlay();
    EnsureSupportComponents();
}

void UDynamicCircuitExecutorComponent::EnsureSupportComponents()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("DynamicCircuitExecutor has no owner."));
        return;
    }

    if (!CircuitVisualizer)
    {
        CircuitVisualizer = NewObject<UQuantumCircuitVisualizerComponent>(Owner);
        if (CircuitVisualizer)
        {
            CircuitVisualizer->RegisterComponent();
            CircuitVisualizer->InitializeCircuitVisualization();
        }
    }

    if (!SuperpositionAnimator)
    {
        SuperpositionAnimator = NewObject<USuperpositionAnimatorComponent>(Owner);
        if (SuperpositionAnimator)
        {
            SuperpositionAnimator->RegisterComponent();
        }
    }
}

void UDynamicCircuitExecutorComponent::ExecuteCircuitWithVisualization()
{
    UE_LOG(LogTemp, Log, TEXT("Executing dynamic circuit visualization..."));
    StepIndex = 0;
    RunNextStep();
}

void UDynamicCircuitExecutorComponent::RunNextStep()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    switch (StepIndex)
    {
    case 0:
        if (CircuitVisualizer)
        {
            CircuitVisualizer->AddGate(TEXT("H"), 0, -1);
        }
        if (SuperpositionAnimator)
        {
            SuperpositionAnimator->AnimateSuperposition(0, 5.0f);
        }
        StepIndex++;
        World->GetTimerManager().SetTimer(StepTimerHandle, this, &UDynamicCircuitExecutorComponent::RunNextStep, StepDelaySeconds, false);
        break;

    case 1:
        if (CircuitVisualizer)
        {
            CircuitVisualizer->AddGate(TEXT("CNOT"), 1, 0);
        }
        StepIndex++;
        World->GetTimerManager().SetTimer(StepTimerHandle, this, &UDynamicCircuitExecutorComponent::RunNextStep, StepDelaySeconds, false);
        break;

    default:
        UE_LOG(LogTemp, Log, TEXT("Circuit execution completed."));
        World->GetTimerManager().ClearTimer(StepTimerHandle);
        break;
    }
}
