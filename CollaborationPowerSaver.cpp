#include "CollaborationPowerSaver.h"

#include "Engine/World.h"
#include "TimerManager.h"

ACollaborationPowerSaver::ACollaborationPowerSaver()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void ACollaborationPowerSaver::BeginPlay()
{
    Super::BeginPlay();

    InitializeSystem();

    if (UWorld* World = GetWorld())
    {
        // Periodically adjust energy rate
        World->GetTimerManager().SetTimer(
            EnergyAdjustmentTimerHandle,
            this,
            &ACollaborationPowerSaver::HandleEnergyAdjustmentTick,
            FMath::Max(0.0f, EnergyAdjustmentInterval),
            true);

        // Periodically stabilize frequencies
        World->GetTimerManager().SetTimer(
            StabilizeFrequencyTimerHandle,
            this,
            &ACollaborationPowerSaver::HandleStabilizeFrequencyTick,
            FMath::Max(0.0f, StabilizeFrequencyInterval),
            true);

        // Periodically generate frequency commands
        World->GetTimerManager().SetTimer(
            FrequencyCommandTimerHandle,
            this,
            &ACollaborationPowerSaver::HandleFrequencyCommandTick,
            FMath::Max(0.0f, FrequencyCommandInterval),
            true);

        // Periodically contain frequencies
        World->GetTimerManager().SetTimer(
            ContainFrequenciesTimerHandle,
            this,
            &ACollaborationPowerSaver::HandleContainFrequenciesTick,
            FMath::Max(0.0f, ContainFrequenciesInterval),
            true);
    }
}

void ACollaborationPowerSaver::InitializeSystem()
{
    CurrentEnergyRate = EnergyRate;
    UE_LOG(LogTemp, Log, TEXT("Collaboration Power Saver Initialized"));
    UpdateAtomicRingFrequency();
}

void ACollaborationPowerSaver::AdjustEnergyRate(float Adjustment)
{
    CurrentEnergyRate = FMath::Clamp(CurrentEnergyRate + Adjustment, 0.0f, 100.0f);
    UE_LOG(LogTemp, Log, TEXT("Energy rate adjusted to %.2f%%"), CurrentEnergyRate);
}

void ACollaborationPowerSaver::UpdateAtomicRingFrequency()
{
    AtomicRingFrequency = (BaseFrequency + SubFrequency) / 2.0f;
    UE_LOG(LogTemp, Log, TEXT("Atomic Ring Frequency set to %.2f Hz"), AtomicRingFrequency);
}

void ACollaborationPowerSaver::StabilizeFrequencies()
{
    const float Balance = (BaseFrequency * 0.52f) + (SubFrequency * 0.48f);
    RecycleRate = Balance * OscillationRate / 100.0f;
    AtomicPressure *= 1.0f - (StabilizerReduction / 100.0f);

    UE_LOG(LogTemp, Log, TEXT("Stabilized frequencies. Recycle Rate: %.2f%%"), RecycleRate);
    UE_LOG(LogTemp, Log, TEXT("Atomic Pressure adjusted to %.2f"), AtomicPressure);
}

void ACollaborationPowerSaver::GenerateFrequencyCommand()
{
    const float TimeSec = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    const float S = FMath::Sin(TimeSec);
    const float FrequencyCommand = S * S; // sin^2(t)
    UE_LOG(LogTemp, Log, TEXT("Generated Frequency Command: %.4f"), FrequencyCommand);
}

void ACollaborationPowerSaver::ContainFrequencies()
{
    UE_LOG(LogTemp, Log, TEXT("Containing all frequencies inside atomic chamber..."));
    UE_LOG(LogTemp, Log, TEXT("Temporal Atomic Time Control Active at %.2f Hz"), AtomicRingFrequency);
}

// Timers
void ACollaborationPowerSaver::HandleEnergyAdjustmentTick()
{
    AdjustEnergyRate(-2.0f); // Example decrease
}

void ACollaborationPowerSaver::HandleStabilizeFrequencyTick()
{
    StabilizeFrequencies();
}

void ACollaborationPowerSaver::HandleFrequencyCommandTick()
{
    GenerateFrequencyCommand();
}

void ACollaborationPowerSaver::HandleContainFrequenciesTick()
{
    ContainFrequencies();
}

