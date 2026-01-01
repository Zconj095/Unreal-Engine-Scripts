#include "MatrixChoiceSystem.h"

#include "Misc/Guid.h"
#include "Engine/World.h"
#include "TimerManager.h"

AMatrixChoiceSystem::AMatrixChoiceSystem()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixChoiceSystem::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Choice System Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddChoiceTimerHandle,
            this,
            &AMatrixChoiceSystem::HandleAddChoiceTick,
            FMath::Max(0.0f, AddChoiceInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveChoiceTimerHandle,
            this,
            &AMatrixChoiceSystem::HandleRetrieveChoiceTick,
            FMath::Max(0.0f, RetrieveChoiceInterval),
            true);

        World->GetTimerManager().SetTimer(
            ValidateChoiceTimerHandle,
            this,
            &AMatrixChoiceSystem::HandleValidateChoiceTick,
            FMath::Max(0.0f, ValidateChoiceInterval),
            true);

        World->GetTimerManager().SetTimer(
            HandleChoiceTimerHandle,
            this,
            &AMatrixChoiceSystem::HandleHandleChoiceTick,
            FMath::Max(0.0f, HandleChoiceInterval),
            true);
    }
}

void AMatrixChoiceSystem::AddChoice(const FString& Description, bool bIsValid)
{
    const FString ChoiceID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
    ChoiceDatabase.Add(ChoiceID, FMatrixChoice(ChoiceID, Description, bIsValid));
    UE_LOG(LogTemp, Log, TEXT("Added Choice - ID: %s, Description: %s, Valid: %s"), *ChoiceID, *Description, bIsValid ? TEXT("true") : TEXT("false"));
}

void AMatrixChoiceSystem::RetrieveChoice(const FString& ChoiceID) const
{
    if (const FMatrixChoice* Choice = ChoiceDatabase.Find(ChoiceID))
    {
        UE_LOG(LogTemp, Log, TEXT("Retrieved Choice - ID: %s, Description: %s, Valid: %s"), *Choice->ChoiceID, *Choice->Description, Choice->bIsValid ? TEXT("true") : TEXT("false"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Choice with ID '%s' not found."), *ChoiceID);
    }
}

bool AMatrixChoiceSystem::ValidateChoiceStability(const FString& ChoiceID) const
{
    if (const FMatrixChoice* Choice = ChoiceDatabase.Find(ChoiceID))
    {
        if (Choice->bIsValid)
        {
            UE_LOG(LogTemp, Log, TEXT("Choice '%s' is stable."), *ChoiceID);
            return true;
        }
        UE_LOG(LogTemp, Warning, TEXT("Choice '%s' is unstable."), *ChoiceID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Choice '%s' does not exist."), *ChoiceID);
    }
    return false;
}

void AMatrixChoiceSystem::HandleChoice(const FString& ChoiceID)
{
    if (ValidateChoiceStability(ChoiceID))
    {
        UE_LOG(LogTemp, Log, TEXT("Handling task for choice ID: %s"), *ChoiceID);
        // Extend handling logic as needed
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Task cannot be handled due to choice instability."));
    }
}

FString AMatrixChoiceSystem::GetLastKey() const
{
    FString LastKey;
    for (const TPair<FString, FMatrixChoice>& Pair : ChoiceDatabase)
    {
        LastKey = Pair.Key; // iteration order is not defined; mirrors Unity sample intent
    }
    return LastKey;
}

// Timers
void AMatrixChoiceSystem::HandleAddChoiceTick()
{
    const bool bValid = FMath::FRand() > 0.5f;
    AddChoice(TEXT("Automated Choice: Enable Matrix Optimization"), bValid);
}

void AMatrixChoiceSystem::HandleRetrieveChoiceTick() const
{
    if (ChoiceDatabase.Num() > 0)
    {
        const FString LastKey = GetLastKey();
        RetrieveChoice(LastKey);
    }
}

void AMatrixChoiceSystem::HandleValidateChoiceTick() const
{
    if (ChoiceDatabase.Num() > 0)
    {
        const FString LastKey = GetLastKey();
        ValidateChoiceStability(LastKey);
    }
}

void AMatrixChoiceSystem::HandleHandleChoiceTick()
{
    if (ChoiceDatabase.Num() > 0)
    {
        const FString LastKey = GetLastKey();
        HandleChoice(LastKey);
    }
}

