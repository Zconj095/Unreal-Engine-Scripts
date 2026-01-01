#include "CoordinationSystem.h"

#include "Engine/World.h"
#include "TimerManager.h"

ACoordinationSystem::ACoordinationSystem()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void ACoordinationSystem::BeginPlay()
{
    Super::BeginPlay();

    InitializeSensorySystem();
    UE_LOG(LogTemp, Log, TEXT("Coordination System Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            SensoryUpdateTimerHandle,
            this,
            &ACoordinationSystem::HandleSensoryUpdateTick,
            FMath::Max(0.0f, SensoryUpdateInterval),
            true);

        World->GetTimerManager().SetTimer(
            MuscleActivityTimerHandle,
            this,
            &ACoordinationSystem::HandleMuscleActivityTick,
            FMath::Max(0.0f, MuscleActivityInterval),
            true);

        World->GetTimerManager().SetTimer(
            VitaminProductionTimerHandle,
            this,
            &ACoordinationSystem::HandleVitaminProductionTick,
            FMath::Max(0.0f, VitaminProductionInterval),
            true);

        World->GetTimerManager().SetTimer(
            TemporalModificationTimerHandle,
            this,
            &ACoordinationSystem::HandleTemporalModificationTick,
            FMath::Max(0.0f, TemporalModificationInterval),
            true);

        World->GetTimerManager().SetTimer(
            QuantumEntanglementTimerHandle,
            this,
            &ACoordinationSystem::HandleQuantumEntanglementTick,
            FMath::Max(0.0f, QuantumEntanglementInterval),
            true);
    }
}

void ACoordinationSystem::InitializeSensorySystem()
{
    SensoryInputs.Empty();
    SensoryInputs.Add(TEXT("Visual"), 0.f);
    SensoryInputs.Add(TEXT("Auditory"), 0.f);
    SensoryInputs.Add(TEXT("Tactile"), 0.f);
    SensoryInputs.Add(TEXT("Olfactory"), 0.f);
    SensoryInputs.Add(TEXT("Gustatory"), 0.f);
    SensoryInputs.Add(TEXT("Emotional"), 0.f);
}

void ACoordinationSystem::UpdateSensoryInput(FName InputType, float Value)
{
    if (float* Found = SensoryInputs.Find(InputType))
    {
        *Found = Value;
        UE_LOG(LogTemp, Log, TEXT("Updated %s sensory input to %.3f"), *InputType.ToString(), Value);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sensory input '%s' is not recognized."), *InputType.ToString());
    }
}

void ACoordinationSystem::CalculateMuscleActivity()
{
    float TotalInput = 0.f;
    for (const TPair<FName, float>& Pair : SensoryInputs)
    {
        TotalInput += Pair.Value;
    }

    MuscleGrowthRate = TotalInput * AcceleratedRate;
    MuscleMemoryRate = TotalInput * MediumRate;

    UE_LOG(LogTemp, Log, TEXT("Calculated Muscle Growth Rate: %.3f"), MuscleGrowthRate);
    UE_LOG(LogTemp, Log, TEXT("Calculated Muscle Memory Rate: %.3f"), MuscleMemoryRate);
}

void ACoordinationSystem::ProduceVitaminsThroughSound()
{
    const float SoundEnergy = BaseFrequency * MediumRate;
    UE_LOG(LogTemp, Log, TEXT("Produced vitamins with sound energy: %.3f"), SoundEnergy);
}

void ACoordinationSystem::ApplyTemporalModifications()
{
    const float TemporalEnergy = BaseFrequency * AcceleratedRate;
    UE_LOG(LogTemp, Log, TEXT("Applied temporal modifications with energy: %.3f"), TemporalEnergy);
}

void ACoordinationSystem::BackupData(const FString& Record)
{
    DataBackup.Add(Record);
    UE_LOG(LogTemp, Log, TEXT("Data backed up: %s"), *Record);
}

void ACoordinationSystem::SimulateQuantumEntanglement()
{
    const float EntangledEnergy = BaseFrequency * AcceleratedRate * MediumRate;
    UE_LOG(LogTemp, Log, TEXT("Simulated quantum entanglement with energy: %.3f"), EntangledEnergy);
    BackupData(FString::Printf(TEXT("Quantum Entanglement Energy: %.3f"), EntangledEnergy));
}

// Timers
void ACoordinationSystem::HandleSensoryUpdateTick()
{
    // Update Visual input with random value [0.1, 1.0]
    UpdateSensoryInput(TEXT("Visual"), FMath::FRandRange(0.1f, 1.0f));
}

void ACoordinationSystem::HandleMuscleActivityTick()
{
    CalculateMuscleActivity();
}

void ACoordinationSystem::HandleVitaminProductionTick()
{
    ProduceVitaminsThroughSound();
}

void ACoordinationSystem::HandleTemporalModificationTick()
{
    ApplyTemporalModifications();
}

void ACoordinationSystem::HandleQuantumEntanglementTick()
{
    SimulateQuantumEntanglement();
}

