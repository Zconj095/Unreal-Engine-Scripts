#include "CalibrationServer.h"

#include "Engine/World.h"
#include "TimerManager.h"

ACalibrationServer::ACalibrationServer()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void ACalibrationServer::BeginPlay()
{
    Super::BeginPlay();

    InitializeSystem();

    // Set timers for automation
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CalibrationTimerHandle,
            this,
            &ACalibrationServer::HandleCalibrationTick,
            FMath::Max(0.0f, CalibrationInterval),
            true);

        World->GetTimerManager().SetTimer(
            GhostCommandTimerHandle,
            this,
            &ACalibrationServer::HandleGhostCommandTick,
            FMath::Max(0.0f, GhostCommandInterval),
            true);

        World->GetTimerManager().SetTimer(
            SensoryAdjustmentTimerHandle,
            this,
            &ACalibrationServer::HandleSensoryAdjustmentTick,
            FMath::Max(0.0f, SensoryAdjustmentInterval),
            true);
    }
}

void ACalibrationServer::InitializeSystem()
{
    // Initialize sensory system defaults
    Sensors.Add(TEXT("Visual"), 1.f);
    Sensors.Add(TEXT("Auditory"), 1.f);
    Sensors.Add(TEXT("Tactile"), 1.f);
    Sensors.Add(TEXT("Olfactory"), 1.f);
    Sensors.Add(TEXT("Gustatory"), 1.f);
    Sensors.Add(TEXT("Temporal"), 1.f);

    // Initial ghost commands
    GhostFrequencyCommands.Add(FGhostFrequencyCommand(SubFrequency, TEXT("Initiate Sleep Calibration")));
    GhostFrequencyCommands.Add(FGhostFrequencyCommand(BaseFrequency, TEXT("Adapt to Personal Frequency")));

    UE_LOG(LogTemp, Log, TEXT("Calibration Server Initialized."));
}

float ACalibrationServer::AdaptedFrequency(float InBaseFrequency, float PersonalFactor) const
{
    // base + personalFactor * TimingOscillation * pow(TimingOscillation, TimingPower)
    const float Term = TimingOscillation * FMath::Pow(TimingOscillation, TimingPower);
    return InBaseFrequency + PersonalFactor * Term;
}

void ACalibrationServer::TestFrequencyAdaptation(float PersonalFactor)
{
    const float Adapted = AdaptedFrequency(BaseFrequency, PersonalFactor);
    UE_LOG(LogTemp, Log, TEXT("Adapted frequency for personal factor %.3f: %.3f Hz"), PersonalFactor, Adapted);
}

void ACalibrationServer::ExecuteGhostCommand(float Frequency)
{
    for (const FGhostFrequencyCommand& Command : GhostFrequencyCommands)
    {
        if (FMath::IsNearlyEqual(Command.Frequency, Frequency, KINDA_SMALL_NUMBER))
        {
            Command.Execute();
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No ghost command found for frequency: %.3f Hz"), Frequency);
}

void ACalibrationServer::AdjustSensorySystem(FName Sensor, float Adjustment)
{
    if (float* Value = Sensors.Find(Sensor))
    {
        *Value = Adjustment;
        UE_LOG(LogTemp, Log, TEXT("Adjusted %s sensor to %.3f"), *Sensor.ToString(), Adjustment);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sensor '%s' not found."), *Sensor.ToString());
    }
}

void ACalibrationServer::HandleCalibrationTick()
{
    // Mirrors Unity: TestFrequencyAdaptation(0.5f)
    TestFrequencyAdaptation(0.5f);
}

void ACalibrationServer::HandleGhostCommandTick()
{
    ExecuteGhostCommand(SubFrequency);
}

void ACalibrationServer::HandleSensoryAdjustmentTick()
{
    // Random adjustment between 0.8 and 1.5 on the Visual sensor
    const float RandomAdjustment = FMath::FRandRange(0.8f, 1.5f);
    AdjustSensorySystem(TEXT("Visual"), RandomAdjustment);
}

