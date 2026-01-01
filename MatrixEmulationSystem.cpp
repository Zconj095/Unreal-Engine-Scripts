#include "MatrixEmulationSystem.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixEmulationSystem::AMatrixEmulationSystem()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixEmulationSystem::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Emulation System Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddEmulatorTimerHandle,
            this,
            &AMatrixEmulationSystem::HandleAddEmulatorTick,
            FMath::Max(0.0f, AddEmulatorInterval),
            true);

        World->GetTimerManager().SetTimer(
            StartEmulatorTimerHandle,
            this,
            &AMatrixEmulationSystem::HandleStartEmulatorTick,
            FMath::Max(0.0f, StartEmulatorInterval),
            true);

        World->GetTimerManager().SetTimer(
            StopEmulatorTimerHandle,
            this,
            &AMatrixEmulationSystem::HandleStopEmulatorTick,
            FMath::Max(0.0f, StopEmulatorInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveStatusTimerHandle,
            this,
            &AMatrixEmulationSystem::HandleRetrieveStatusTick,
            FMath::Max(0.0f, RetrieveStatusInterval),
            true);
    }
}

void AMatrixEmulationSystem::AddEmulator(const FString& TaskDescription, const FString& ControlledSystem)
{
    FMatrixEmulatorData Emulator(TaskDescription, ControlledSystem);
    UE_LOG(LogTemp, Log, TEXT("Emulator Added - ID: %s, Task: %s, System: %s"), *Emulator.EmulatorID, *TaskDescription, *ControlledSystem);
    Emulators.Add(MoveTemp(Emulator));
}

void AMatrixEmulationSystem::StartEmulator(const FString& EmulatorID)
{
    for (FMatrixEmulatorData& Emulator : Emulators)
    {
        if (Emulator.EmulatorID == EmulatorID)
        {
            if (!Emulator.bIsRunning)
            {
                Emulator.Start();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Emulator '%s' is already running."), *EmulatorID);
            }
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Emulator with ID '%s' not found."), *EmulatorID);
}

void AMatrixEmulationSystem::StopEmulator(const FString& EmulatorID)
{
    for (FMatrixEmulatorData& Emulator : Emulators)
    {
        if (Emulator.EmulatorID == EmulatorID)
        {
            if (Emulator.bIsRunning)
            {
                Emulator.Stop();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Emulator '%s' is not running."), *EmulatorID);
            }
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Emulator with ID '%s' not found."), *EmulatorID);
}

void AMatrixEmulationSystem::RetrieveEmulatorStatus() const
{
    UE_LOG(LogTemp, Log, TEXT("Retrieving status of all emulators..."));
    for (const FMatrixEmulatorData& Emulator : Emulators)
    {
        UE_LOG(LogTemp, Log, TEXT("Emulator - ID: %s, Task: %s, System: %s, Running: %s"), *Emulator.EmulatorID, *Emulator.TaskDescription, *Emulator.ControlledSystem, Emulator.bIsRunning ? TEXT("true") : TEXT("false"));
    }
}

void AMatrixEmulationSystem::ControlAllEmulators(bool bStart)
{
    UE_LOG(LogTemp, Log, TEXT("%s all emulators..."), bStart ? TEXT("Starting") : TEXT("Stopping"));
    for (FMatrixEmulatorData& Emulator : Emulators)
    {
        if (bStart && !Emulator.bIsRunning)
        {
            Emulator.Start();
        }
        else if (!bStart && Emulator.bIsRunning)
        {
            Emulator.Stop();
        }
    }
}

// Timers
void AMatrixEmulationSystem::HandleAddEmulatorTick()
{
    const int32 Count = Emulators.Num() + 1;
    AddEmulator(FString::Printf(TEXT("Task_%d"), Count), FString::Printf(TEXT("System_%d"), Count));
}

void AMatrixEmulationSystem::HandleStartEmulatorTick()
{
    for (FMatrixEmulatorData& Emulator : Emulators)
    {
        if (!Emulator.bIsRunning)
        {
            Emulator.Start();
            break; // Start one emulator at a time
        }
    }
}

void AMatrixEmulationSystem::HandleStopEmulatorTick()
{
    for (FMatrixEmulatorData& Emulator : Emulators)
    {
        if (Emulator.bIsRunning)
        {
            Emulator.Stop();
            break; // Stop one emulator at a time
        }
    }
}

void AMatrixEmulationSystem::HandleRetrieveStatusTick() const
{
    RetrieveEmulatorStatus();
}
