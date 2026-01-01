#include "InfinityMatrixPowerSupply.h"

#include "Engine/World.h"
#include "TimerManager.h"

AInfinityMatrixPowerSupply::AInfinityMatrixPowerSupply()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AInfinityMatrixPowerSupply::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Infinity Matrix Power Supply Initialized."));
    InitializeDataFlow();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            PathSetTimerHandle,
            this,
            &AInfinityMatrixPowerSupply::HandlePathSetTick,
            FMath::Max(0.0f, PathSetInterval),
            true);

        World->GetTimerManager().SetTimer(
            VoltageAdjustmentTimerHandle,
            this,
            &AInfinityMatrixPowerSupply::HandleVoltageAdjustmentTick,
            FMath::Max(0.0f, VoltageAdjustmentInterval),
            true);

        World->GetTimerManager().SetTimer(
            DataProcessingTimerHandle,
            this,
            &AInfinityMatrixPowerSupply::HandleDataProcessingTick,
            FMath::Max(0.0f, DataProcessingInterval),
            true);

        World->GetTimerManager().SetTimer(
            ElectricalCommandTimerHandle,
            this,
            &AInfinityMatrixPowerSupply::HandleElectricalCommandTick,
            FMath::Max(0.0f, ElectricalCommandInterval),
            true);
    }
}

void AInfinityMatrixPowerSupply::InitializeDataFlow()
{
    DataFlow.Add(TEXT("Matrix Cortex"), TEXT("Receiving Data Commands"));
    DataFlow.Add(TEXT("Collaboration Chamber"), TEXT("Synchronizing Data"));
    DataFlow.Add(TEXT("Matrix Emulator"), TEXT("Emulating Commands"));
    DataFlow.Add(TEXT("Matrix Simulator"), TEXT("Simulating Outputs"));
    UE_LOG(LogTemp, Log, TEXT("Data flow pathways established."));
}

void AInfinityMatrixPowerSupply::SetFrequencyPath(int32 PathNumber)
{
    switch (PathNumber)
    {
        case 1:
            CurrentPath = EFrequencyPath::PathOne;
            UE_LOG(LogTemp, Log, TEXT("Set Frequency Path to Path One."));
            break;
        case 2:
            CurrentPath = EFrequencyPath::PathTwo;
            UE_LOG(LogTemp, Log, TEXT("Set Frequency Path to Path Two."));
            break;
        case 3:
            CurrentPath = EFrequencyPath::PathThree;
            UE_LOG(LogTemp, Log, TEXT("Set Frequency Path to Path Three."));
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Invalid path number."));
            break;
    }
}

void AInfinityMatrixPowerSupply::AdjustVoltage(float Adjustment)
{
    VoltageLevel = FMath::Clamp(VoltageLevel + Adjustment, 0.0f, 100.0f);
    UE_LOG(LogTemp, Log, TEXT("Voltage level adjusted to %.2fV"), VoltageLevel);
}

void AInfinityMatrixPowerSupply::ProcessData()
{
    for (const TPair<FString, FString>& Pair : DataFlow)
    {
        UE_LOG(LogTemp, Log, TEXT("Processing %s: %s"), *Pair.Key, *Pair.Value);
    }
}

void AInfinityMatrixPowerSupply::DistributeElectricalCommands()
{
    const float PowerOutput = VoltageLevel * 0.8f; // Example distribution rate
    UE_LOG(LogTemp, Log, TEXT("Distributing electrical commands with power output: %.2fW"), PowerOutput);
}

// Timers
void AInfinityMatrixPowerSupply::HandlePathSetTick()
{
    const int32 PathNumber = FMath::RandRange(1, 3); // inclusive
    SetFrequencyPath(PathNumber);
}

void AInfinityMatrixPowerSupply::HandleVoltageAdjustmentTick()
{
    const float Adjustment = FMath::FRandRange(-5.0f, 10.0f);
    AdjustVoltage(Adjustment);
}

void AInfinityMatrixPowerSupply::HandleDataProcessingTick()
{
    ProcessData();
}

void AInfinityMatrixPowerSupply::HandleElectricalCommandTick()
{
    DistributeElectricalCommands();
}

