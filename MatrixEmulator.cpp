#include "MatrixEmulator.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixEmulator::AMatrixEmulator()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixEmulator::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Emulator Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddReplicaTimerHandle,
            this,
            &AMatrixEmulator::HandleAddReplicaTick,
            FMath::Max(0.0f, AddReplicaInterval),
            true);

        World->GetTimerManager().SetTimer(
            ActivateReplicaTimerHandle,
            this,
            &AMatrixEmulator::HandleActivateReplicaTick,
            FMath::Max(0.0f, ActivateReplicaInterval),
            true);

        World->GetTimerManager().SetTimer(
            DeactivateReplicaTimerHandle,
            this,
            &AMatrixEmulator::HandleDeactivateReplicaTick,
            FMath::Max(0.0f, DeactivateReplicaInterval),
            true);

        World->GetTimerManager().SetTimer(
            AdjustFluxTimerHandle,
            this,
            &AMatrixEmulator::HandleAdjustFluxTick,
            FMath::Max(0.0f, AdjustFluxInterval),
            true);

        World->GetTimerManager().SetTimer(
            SimulateDataFlowTimerHandle,
            this,
            &AMatrixEmulator::HandleSimulateDataFlowTick,
            FMath::Max(0.0f, SimulateDataFlowInterval),
            true);
    }
}

void AMatrixEmulator::AddReplica(const FString& OriginalData, const FString& SimulatedSpace)
{
    const FReplica Replica(OriginalData, SimulatedSpace);
    Replicas.Add(Replica);
    UE_LOG(LogTemp, Log, TEXT("Added Replica - ID: %s, Original Data: %s, Simulated Space: %s"), *Replica.ReplicaID, *OriginalData, *SimulatedSpace);
}

void AMatrixEmulator::ActivateReplica(const FString& ReplicaID)
{
    for (FReplica& Replica : Replicas)
    {
        if (Replica.ReplicaID == ReplicaID)
        {
            if (!Replica.bIsActive)
            {
                Replica.Activate();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Replica '%s' is already active."), *ReplicaID);
            }
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Replica with ID '%s' not found."), *ReplicaID);
}

void AMatrixEmulator::DeactivateReplica(const FString& ReplicaID)
{
    for (FReplica& Replica : Replicas)
    {
        if (Replica.ReplicaID == ReplicaID)
        {
            if (Replica.bIsActive)
            {
                Replica.Deactivate();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Replica '%s' is already inactive."), *ReplicaID);
            }
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Replica with ID '%s' not found."), *ReplicaID);
}

void AMatrixEmulator::AdjustFluxCapacitance(float Adjustment)
{
    FluxCapacitancePower = FMath::Clamp(FluxCapacitancePower + Adjustment, 0.0f, 100.0f);
    UE_LOG(LogTemp, Log, TEXT("Flux Capacitance Power adjusted to %.2f%%."), FluxCapacitancePower);
}

void AMatrixEmulator::SimulateDataFlow() const
{
    if (FluxCapacitancePower <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient flux capacitance power for data simulation."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Simulating data flow between replicas and virtual grid..."));
    for (const FReplica& Replica : Replicas)
    {
        if (Replica.bIsActive)
        {
            UE_LOG(LogTemp, Log, TEXT("Simulated Data Flow - Replica ID: %s, Space: %s"), *Replica.ReplicaID, *Replica.SimulatedSpace);
        }
    }
}

// Timers
void AMatrixEmulator::HandleAddReplicaTick()
{
    const int32 Count = Replicas.Num() + 1;
    AddReplica(FString::Printf(TEXT("OriginalData_%d"), Count), FString::Printf(TEXT("VirtualSpace_%d"), Count));
}

void AMatrixEmulator::HandleActivateReplicaTick()
{
    for (FReplica& Replica : Replicas)
    {
        if (!Replica.bIsActive)
        {
            Replica.Activate();
            break; // Activate one at a time
        }
    }
}

void AMatrixEmulator::HandleDeactivateReplicaTick()
{
    for (FReplica& Replica : Replicas)
    {
        if (Replica.bIsActive)
        {
            Replica.Deactivate();
            break; // Deactivate one at a time
        }
    }
}

void AMatrixEmulator::HandleAdjustFluxTick()
{
    const float Adjustment = FMath::FRandRange(-10.f, 10.f);
    AdjustFluxCapacitance(Adjustment);
}

void AMatrixEmulator::HandleSimulateDataFlowTick() const
{
    SimulateDataFlow();
}

