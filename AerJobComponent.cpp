// Copyright Epic Games, Inc.

#include "AerJobComponent.h"

#include "AerSimulator.h"
#include "Async/Async.h"

UAerJobComponent::UAerJobComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAerJobComponent::ConfigureJob(const FString& InJobId, const TArray<FString>& InCircuit, int32 InNumQubits)
{
    JobId = InJobId;
    Circuit = InCircuit;
    NumQubits = InNumQubits;
}

void UAerJobComponent::Execute(UAerSimulator* Simulator)
{
    if (!Simulator)
    {
        UE_LOG(LogTemp, Error, TEXT("AerSimulator instance is null. Ensure it is initialized before executing the job."));
        Result.Reset();
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Executing job %s..."), *JobId);

    const TArray<FString> CircuitCopy = Circuit;
    const int32 QubitCopy = NumQubits;
    TWeakObjectPtr<UAerJobComponent> WeakThis(this);

    Async(EAsyncExecution::ThreadPool, [WeakThis, Simulator, CircuitCopy, QubitCopy]()
    {
        const TMap<FString, float> SimulationResult = Simulator->SimulateCircuit(CircuitCopy, QubitCopy);

        AsyncTask(ENamedThreads::GameThread, [WeakThis, SimulationResult]()
        {
            if (!WeakThis.IsValid())
            {
                return;
            }

            WeakThis->Result = SimulationResult;

            if (SimulationResult.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Job %s completed with empty results."), *WeakThis->JobId);
        }

        UE_LOG(LogTemp, Log, TEXT("Job %s completed."), *WeakThis->JobId);
        for (const TPair<FString, float>& Pair : SimulationResult)
        {
            UE_LOG(LogTemp, Log, TEXT("State: %s, Probability: %f"), *Pair.Key, Pair.Value);
        }

        WeakThis->OnJobCompleted.Broadcast(WeakThis.Get());
    });
});
}
