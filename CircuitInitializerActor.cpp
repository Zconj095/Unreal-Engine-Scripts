// Copyright Epic Games, Inc.

#include "CircuitInitializerActor.h"

#include "AncillaManagerComponent.h"
#include "CircuitAnalysisPassComponent.h"
#include "Components/SceneComponent.h"

ACircuitInitializerActor::ACircuitInitializerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    AncillaManager = CreateDefaultSubobject<UAncillaManagerComponent>(TEXT("AncillaManager"));
    AnalysisPass = CreateDefaultSubobject<UCircuitAnalysisPassComponent>(TEXT("CircuitAnalysisPass"));
}

void ACircuitInitializerActor::BeginPlay()
{
    Super::BeginPlay();

    if (AncillaManager)
    {
        AncillaManager->PrimaryQubitCount = NumQubits;
        AncillaManager->AncillaQubitCount = NumAncillas;
        AncillaManager->InitializeRegisters();
    }

    UE_LOG(LogTemp, Log, TEXT("Circuit Initialized with %d qubits and %d ancillas."), NumQubits, NumAncillas);
    UE_LOG(LogTemp, Log, TEXT("Initial State: %s"), *InitialState);
}

void ACircuitInitializerActor::AddGate(const FString& Type, const FString& Qubit)
{
    if (!AnalysisPass)
    {
        UE_LOG(LogTemp, Warning, TEXT("CircuitAnalysisPass component missing; cannot add gate."));
        return;
    }

    TArray<FString> Qubits;
    Qubits.Add(Qubit);
    AnalysisPass->AddGate(Type, Qubits);
    UE_LOG(LogTemp, Log, TEXT("Added Gate: %s on Qubit: %s"), *Type, *Qubit);
}

void ACircuitInitializerActor::RunAnalysis()
{
    if (!AnalysisPass)
    {
        UE_LOG(LogTemp, Warning, TEXT("CircuitAnalysisPass component missing; cannot run analysis."));
        return;
    }

    AnalysisPass->AnalyzeCircuit();
}
