#include "MatrixDomain.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixDomain::AMatrixDomain()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixDomain::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Domain System Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddDomainTimerHandle,
            this,
            &AMatrixDomain::HandleAddDomainTick,
            FMath::Max(0.0f, AddDomainInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveDomainsTimerHandle,
            this,
            &AMatrixDomain::HandleRetrieveDomainsTick,
            FMath::Max(0.0f, RetrieveDomainsInterval),
            true);

        World->GetTimerManager().SetTimer(
            SynchronizeDomainsTimerHandle,
            this,
            &AMatrixDomain::HandleSynchronizeDomainsTick,
            FMath::Max(0.0f, SynchronizeDomainsInterval),
            true);

        World->GetTimerManager().SetTimer(
            AdjustDomainTimerHandle,
            this,
            &AMatrixDomain::HandleAdjustDomainTick,
            FMath::Max(0.0f, AdjustDomainInterval),
            true);
    }
}

void AMatrixDomain::AddDomain(const FString& Word, const FString& Code)
{
    const FDomain NewDomain(Word, Code);
    Domains.Add(NewDomain);
    UE_LOG(LogTemp, Log, TEXT("Added Domain - ID: %s, Word: %s, Code: %s, Synchronized: %s"), *NewDomain.DomainID, *Word, *Code, NewDomain.bIsSynchronized ? TEXT("true") : TEXT("false"));
}

void AMatrixDomain::SynchronizeDomains()
{
    UE_LOG(LogTemp, Log, TEXT("Synchronizing all domains..."));
    for (FDomain& Domain : Domains)
    {
        if (!Domain.bIsSynchronized)
        {
            Domain.ToggleSynchronization();
        }
    }
}

void AMatrixDomain::RetrieveDomains() const
{
    UE_LOG(LogTemp, Log, TEXT("Retrieving all domains..."));
    for (const FDomain& Domain : Domains)
    {
        UE_LOG(LogTemp, Log, TEXT("Domain - ID: %s, Word: %s, Code: %s, Synchronized: %s"), *Domain.DomainID, *Domain.AssociatedWord, *Domain.AssociatedCode, Domain.bIsSynchronized ? TEXT("true") : TEXT("false"));
    }
}

void AMatrixDomain::AdjustDomain(const FString& DomainID, const FString& NewWord, const FString& NewCode)
{
    for (FDomain& Domain : Domains)
    {
        if (Domain.DomainID == DomainID)
        {
            Domain.AssociatedWord = NewWord;
            Domain.AssociatedCode = NewCode;
            UE_LOG(LogTemp, Log, TEXT("Adjusted Domain - ID: %s, New Word: %s, New Code: %s"), *DomainID, *NewWord, *NewCode);
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Domain with ID '%s' not found."), *DomainID);
}

// Timers
void AMatrixDomain::HandleAddDomainTick()
{
    const int32 Count = Domains.Num() + 1;
    AddDomain(FString::Printf(TEXT("Word_%d"), Count), FString::Printf(TEXT("Code_%d"), Count));
}

void AMatrixDomain::HandleSynchronizeDomainsTick()
{
    SynchronizeDomains();
}

void AMatrixDomain::HandleRetrieveDomainsTick() const
{
    RetrieveDomains();
}

void AMatrixDomain::HandleAdjustDomainTick()
{
    if (Domains.Num() > 0)
    {
        const int32 Index = FMath::RandRange(0, Domains.Num() - 1);
        const FString& DomainID = Domains[Index].DomainID;
        const FString NewWord = FString::Printf(TEXT("AdjustedWord_%d"), FMath::RandRange(1, 100));
        const FString NewCode = FString::Printf(TEXT("AdjustedCode_%d"), FMath::RandRange(1, 100));
        AdjustDomain(DomainID, NewWord, NewCode);
    }
}

