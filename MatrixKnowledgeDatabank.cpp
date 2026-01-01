#include "MatrixKnowledgeDatabank.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixKnowledgeDatabank::AMatrixKnowledgeDatabank()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixKnowledgeDatabank::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Knowledge Databank Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddEntryTimerHandle,
            this,
            &AMatrixKnowledgeDatabank::HandleAddEntryTick,
            FMath::Max(0.0f, AddEntryInterval),
            true);

        World->GetTimerManager().SetTimer(
            LinkSectionTimerHandle,
            this,
            &AMatrixKnowledgeDatabank::HandleLinkSectionTick,
            FMath::Max(0.0f, LinkSectionInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveAllEntriesTimerHandle,
            this,
            &AMatrixKnowledgeDatabank::HandleRetrieveAllEntriesTick,
            FMath::Max(0.0f, RetrieveAllEntriesInterval),
            true);

        World->GetTimerManager().SetTimer(
            RetrieveSpecificEntryTimerHandle,
            this,
            &AMatrixKnowledgeDatabank::HandleRetrieveSpecificEntryTick,
            FMath::Max(0.0f, RetrieveSpecificEntryInterval),
            true);
    }
}

void AMatrixKnowledgeDatabank::AddKnowledgeEntry(const FString& Category, const FString& Data)
{
    FKnowledgeEntry Entry(Category, Data);
    Databank.Add(Entry.EntryID, Entry);
    UE_LOG(LogTemp, Log, TEXT("Knowledge Entry Added - ID: %s, Category: %s, Data: %s"), *Entry.EntryID, *Category, *Data);
}

void AMatrixKnowledgeDatabank::LinkEntryToSection(const FString& EntryID, const FString& Section)
{
    if (FKnowledgeEntry* Entry = Databank.Find(EntryID))
    {
        Entry->LinkSection(Section);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Entry with ID '%s' not found."), *EntryID);
    }
}

void AMatrixKnowledgeDatabank::RetrieveEntry(const FString& EntryID) const
{
    if (const FKnowledgeEntry* Entry = Databank.Find(EntryID))
    {
        UE_LOG(LogTemp, Log, TEXT("Entry - ID: %s, Category: %s, Data: %s"), *Entry->EntryID, *Entry->Category, *Entry->Data);
        FString Sections = FString::Join(Entry->LinkedSections, TEXT(", "));
        UE_LOG(LogTemp, Log, TEXT("Linked Sections: %s"), *Sections);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Entry with ID '%s' not found."), *EntryID);
    }
}

void AMatrixKnowledgeDatabank::RetrieveAllEntries() const
{
    UE_LOG(LogTemp, Log, TEXT("Retrieving all knowledge entries..."));
    for (const TPair<FString, FKnowledgeEntry>& Pair : Databank)
    {
        const FKnowledgeEntry& Entry = Pair.Value;
        UE_LOG(LogTemp, Log, TEXT("Entry - ID: %s, Category: %s, Data: %s"), *Entry.EntryID, *Entry.Category, *Entry.Data);
        FString Sections = FString::Join(Entry.LinkedSections, TEXT(", "));
        UE_LOG(LogTemp, Log, TEXT("Linked Sections: %s"), *Sections);
    }
}

// Timers
void AMatrixKnowledgeDatabank::HandleAddEntryTick()
{
    const FString Category = FString::Printf(TEXT("Category_%d"), Databank.Num() + 1);
    const int32 Rand = FMath::RandRange(1, 99);
    const FString Data = FString::Printf(TEXT("Data_%d"), Rand);
    AddKnowledgeEntry(Category, Data);
}

void AMatrixKnowledgeDatabank::HandleLinkSectionTick()
{
    if (Databank.Num() > 0)
    {
        for (TPair<FString, FKnowledgeEntry>& Pair : Databank)
        {
            LinkEntryToSection(Pair.Key, TEXT("AutomatedSection"));
            break; // Link only one entry per interval
        }
    }
}

void AMatrixKnowledgeDatabank::HandleRetrieveAllEntriesTick() const
{
    RetrieveAllEntries();
}

void AMatrixKnowledgeDatabank::HandleRetrieveSpecificEntryTick() const
{
    if (Databank.Num() > 0)
    {
        for (const TPair<FString, FKnowledgeEntry>& Pair : Databank)
        {
            RetrieveEntry(Pair.Key);
            break; // Retrieve only one entry per interval
        }
    }
}

