#include "MatrixCoreSystem.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixCoreSystem::AMatrixCoreSystem()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixCoreSystem::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Core System Initialized."));

    InitializeSensoryDefaults();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddSubsystemTimerHandle,
            this,
            &AMatrixCoreSystem::HandleAddSubsystemTick,
            FMath::Max(0.0f, AddSubsystemInterval),
            true);

        World->GetTimerManager().SetTimer(
            ToggleSubsystemTimerHandle,
            this,
            &AMatrixCoreSystem::HandleToggleSubsystemTick,
            FMath::Max(0.0f, ToggleSubsystemInterval),
            true);

        World->GetTimerManager().SetTimer(
            UpdateSensoryInputTimerHandle,
            this,
            &AMatrixCoreSystem::HandleUpdateSensoryInputTick,
            FMath::Max(0.0f, UpdateSensoryInputInterval),
            true);

        World->GetTimerManager().SetTimer(
            AddFormulaTimerHandle,
            this,
            &AMatrixCoreSystem::HandleAddFormulaTick,
            FMath::Max(0.0f, AddFormulaInterval),
            true);

        World->GetTimerManager().SetTimer(
            ExecuteFormulasTimerHandle,
            this,
            &AMatrixCoreSystem::HandleExecuteFormulasTick,
            FMath::Max(0.0f, ExecuteFormulasInterval),
            true);
    }
}

void AMatrixCoreSystem::InitializeSensoryDefaults()
{
    SensoryInputs.Empty();
    SensoryInputs.Add(TEXT("Visual"), 0.f);
    SensoryInputs.Add(TEXT("Auditory"), 0.f);
    SensoryInputs.Add(TEXT("Tactile"), 0.f);
    SensoryInputs.Add(TEXT("Olfactory"), 0.f);
    SensoryInputs.Add(TEXT("Gustatory"), 0.f);
}

void AMatrixCoreSystem::AddSubsystem(const FString& Name)
{
    if (!LinkedSubsystems.Contains(Name))
    {
        LinkedSubsystems.Add(Name, FMatrixSubsystem(Name, false));
        UE_LOG(LogTemp, Log, TEXT("Subsystem '%s' linked to the Matrix Core."), *Name);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Subsystem '%s' is already linked."), *Name);
    }
}

void AMatrixCoreSystem::ToggleSubsystem(const FString& Name)
{
    if (FMatrixSubsystem* Subsystem = LinkedSubsystems.Find(Name))
    {
        Subsystem->ToggleActivation();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Subsystem '%s' not found."), *Name);
    }
}

void AMatrixCoreSystem::UpdateSensoryInput(const FString& InputType, float Value)
{
    if (float* Found = SensoryInputs.Find(InputType))
    {
        *Found = FMath::Clamp(Value, 0.f, 100.f);
        UE_LOG(LogTemp, Log, TEXT("Sensory input '%s' updated to %.2f%%."), *InputType, *Found);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid sensory input type: '%s'."), *InputType);
    }
}

void AMatrixCoreSystem::AddFormula(const FString& Formula)
{
    Formulas.Add(Formula);
    UE_LOG(LogTemp, Log, TEXT("Formula added to frequency chamber: %s"), *Formula);
}

void AMatrixCoreSystem::ExecuteFormulas() const
{
    UE_LOG(LogTemp, Log, TEXT("Executing formulas in the frequency chamber..."));
    for (const FString& Formula : Formulas)
    {
        UE_LOG(LogTemp, Log, TEXT("Executing Formula: %s"), *Formula);
    }
}

// Timers
void AMatrixCoreSystem::HandleAddSubsystemTick()
{
    const FString Name = FString::Printf(TEXT("Subsystem_%d"), LinkedSubsystems.Num() + 1);
    AddSubsystem(Name);
}

void AMatrixCoreSystem::HandleToggleSubsystemTick()
{
    if (LinkedSubsystems.Num() > 0)
    {
        for (TPair<FString, FMatrixSubsystem>& Pair : LinkedSubsystems)
        {
            Pair.Value.ToggleActivation();
            break; // Toggle one subsystem at a time
        }
    }
}

void AMatrixCoreSystem::HandleUpdateSensoryInputTick()
{
    static const TArray<FString> InputTypes = { TEXT("Visual"), TEXT("Auditory"), TEXT("Tactile"), TEXT("Olfactory"), TEXT("Gustatory") };
    const int32 Index = FMath::RandRange(0, InputTypes.Num() - 1);
    const float NewValue = FMath::FRandRange(10.f, 100.f);
    UpdateSensoryInput(InputTypes[Index], NewValue);
}

void AMatrixCoreSystem::HandleAddFormulaTick()
{
    const FString Formula = FString::Printf(TEXT("Formula_%d: RandomOperation"), Formulas.Num() + 1);
    AddFormula(Formula);
}

void AMatrixCoreSystem::HandleExecuteFormulasTick() const
{
    ExecuteFormulas();
}
