// Copyright Epic Games, Inc.

#include "AerSimulatorExampleActor.h"

#include "AerManagerComponent.h"

AAerSimulatorExampleActor::AAerSimulatorExampleActor()
{
    PrimaryActorTick.bCanEverTick = false;

    AerManager = CreateDefaultSubobject<UAerManagerComponent>(TEXT("AerManager"));
}

void AAerSimulatorExampleActor::BeginPlay()
{
    Super::BeginPlay();

    if (!AerManager)
    {
        UE_LOG(LogTemp, Error, TEXT("AerManager component missing; cannot enqueue jobs."));
        return;
    }

    // Define circuits
    TArray<FString> Circuit1;
    Circuit1.Add(TEXT("H 0"));
    Circuit1.Add(TEXT("CNOT 0 1"));

    TArray<FString> Circuit2;
    Circuit2.Add(TEXT("X 1"));
    Circuit2.Add(TEXT("H 0"));
    Circuit2.Add(TEXT("Z 1"));

    // Add jobs to the manager
    AerManager->AddJob(TEXT("Job1"), Circuit1, 2);
    AerManager->AddJob(TEXT("Job2"), Circuit2, 2);

    // Execute all jobs
    AerManager->ExecuteJobs();
}
