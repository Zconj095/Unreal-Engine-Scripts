// Copyright Epic Games, Inc.

#include "AerManagerComponent.h"

#include "AerJobComponent.h"
#include "AerSimulator.h"
#include "Engine/World.h"

UAerManagerComponent::UAerManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SimulatorClass = UAerSimulator::StaticClass();
}

void UAerManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!SimulatorInstance)
    {
        SimulatorInstance = NewObject<UAerSimulator>(this, SimulatorClass ? *SimulatorClass : UAerSimulator::StaticClass());
    }

    if (SimulatorInstance)
    {
        UE_LOG(LogTemp, Log, TEXT("AerSimulator initialized."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize AerSimulator."));
    }
}

void UAerManagerComponent::AddJob(const FString& JobId, const TArray<FString>& Circuit, int32 NumQubits)
{
    if (JobId.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot add job with empty JobId."));
        return;
    }

    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AerManagerComponent has no owner actor; cannot add job."));
        return;
    }

    UAerJobComponent* Job = NewObject<UAerJobComponent>(OwnerActor);
    if (!Job)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create AerJobComponent for job %s."), *JobId);
        return;
    }

    Job->RegisterComponent();
    Job->ConfigureJob(JobId, Circuit, NumQubits);
    JobQueue.Add(Job);

    UE_LOG(LogTemp, Log, TEXT("Added job %s to the queue."), *JobId);
}

void UAerManagerComponent::ExecuteJobs()
{
    if (bIsExecuting)
    {
        UE_LOG(LogTemp, Warning, TEXT("Job execution already in progress."));
        return;
    }

    if (!SimulatorInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot execute jobs: AerSimulator is not initialized."));
        return;
    }

    bIsExecuting = true;
    ProcessNextJob();
}

void UAerManagerComponent::ClearQueue()
{
    JobQueue.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared queued Aer jobs."));
}

void UAerManagerComponent::ProcessNextJob()
{
    if (JobQueue.Num() == 0)
    {
        bIsExecuting = false;
        UE_LOG(LogTemp, Log, TEXT("No more Aer jobs to execute."));
        return;
    }

    UAerJobComponent* Job = JobQueue[0];
    JobQueue.RemoveAt(0);

    if (!Job)
    {
        UE_LOG(LogTemp, Warning, TEXT("Encountered null job; skipping."));
        ProcessNextJob();
        return;
    }

    Job->OnJobCompleted.AddUObject(this, &UAerManagerComponent::HandleJobCompleted);
    Job->Execute(SimulatorInstance);
}

void UAerManagerComponent::HandleJobCompleted(UAerJobComponent* CompletedJob)
{
    DisplayJobResult(CompletedJob);
    ProcessNextJob();
}

void UAerManagerComponent::DisplayJobResult(UAerJobComponent* Job) const
{
    if (!Job)
    {
        UE_LOG(LogTemp, Error, TEXT("DisplayJobResult called with null job."));
        return;
    }

    if (Job->Result.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Job %s returned null or empty results."), *Job->JobId);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Results for Job %s:"), *Job->JobId);
    for (const TPair<FString, float>& Pair : Job->Result)
    {
        UE_LOG(LogTemp, Log, TEXT("State: %s, Probability: %f"), *Pair.Key, Pair.Value);
    }
}
