#include "InfinityMatrix.h"

#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/World.h"
#include "TimerManager.h"

AInfinityMatrix::AInfinityMatrix()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AInfinityMatrix::BeginPlay()
{
    Super::BeginPlay();

    WordsFolderPath = FPaths::Combine(FPaths::ProjectContentDir(), WordsFolderRelative);
    LoadWordsFromFolder();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddWordTimerHandle,
            this,
            &AInfinityMatrix::HandleAddWordTick,
            FMath::Max(0.0f, AddWordInterval),
            true);

        World->GetTimerManager().SetTimer(
            RemoveWordTimerHandle,
            this,
            &AInfinityMatrix::HandleRemoveWordTick,
            FMath::Max(0.0f, RemoveWordInterval),
            true);
    }
}

void AInfinityMatrix::LoadWordsFromFolder()
{
    if (!FPaths::DirectoryExists(WordsFolderPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Words folder not found at: %s"), *WordsFolderPath);
        return;
    }

    // Gather all files in directory
    TArray<FString> Files;
    IFileManager::Get().FindFilesRecursive(
        Files,
        *WordsFolderPath,
        TEXT("*.*"),
        true,
        false,
        false);

    for (const FString& File : Files)
    {
        const FString Extension = FPaths::GetExtension(File, false).ToLower();
        const FString Word = FPaths::GetBaseFilename(File);
        FString Definition;

        FString Content;
        FFileHelper::LoadFileToString(Content, *File);

        if (Extension == TEXT("md"))
        {
            Definition = Content.TrimStartAndEnd();
        }
        else if (Extension == TEXT("py"))
        {
            int32 EqIdx = INDEX_NONE;
            if (Content.FindChar('=', EqIdx))
            {
                const FString LHS = Content.Left(EqIdx).TrimStartAndEnd();
                FString RHS = Content.Mid(EqIdx + 1).TrimStartAndEnd();
                if (LHS == Word)
                {
                    // Strip surrounding quotes if present
                    if (RHS.StartsWith(TEXT("\"")) && RHS.EndsWith(TEXT("\"")))
                    {
                        RHS = RHS.Mid(1, RHS.Len() - 2);
                    }
                    Definition = RHS;
                }
            }
        }
        else if (Extension == TEXT("bat"))
        {
            const FString Prefix = TEXT("echo");
            if (Content.Left(Prefix.Len()).Equals(Prefix, ESearchCase::IgnoreCase))
            {
                Definition = Content.Mid(Prefix.Len()).TrimStartAndEnd();
            }
        }

        if (!Definition.IsEmpty())
        {
            WordBank.Add(Word, FMatrixWord(Word, Definition));
            UE_LOG(LogTemp, Log, TEXT("Loaded word: %s, Definition: %s"), *Word, *Definition);
        }
    }
}

void AInfinityMatrix::AddWordToMatrix(const FString& Word)
{
    if (const FMatrixWord* Found = WordBank.Find(Word))
    {
        ActiveWords.Add(*Found);
        TotalPower += Found->PowerGenerated;
        UE_LOG(LogTemp, Log, TEXT("Added word to matrix: %s. Total Power: %.2f"), *Word, TotalPower);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Word '%s' not found in the word bank."), *Word);
    }
}

void AInfinityMatrix::RemoveWordFromMatrix(const FString& Word)
{
    const int32 Index = ActiveWords.IndexOfByPredicate([&](const FMatrixWord& W){ return W.Word == Word; });
    if (Index != INDEX_NONE)
    {
        const float Power = ActiveWords[Index].PowerGenerated;
        ActiveWords.RemoveAt(Index);
        TotalPower -= Power;
        UE_LOG(LogTemp, Log, TEXT("Removed word from matrix: %s. Total Power: %.2f"), *Word, TotalPower);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Word '%s' is not active in the matrix."), *Word);
    }
}

// Timers
void AInfinityMatrix::HandleAddWordTick()
{
    if (WordBank.Num() > 0)
    {
        TArray<FString> Keys;
        WordBank.GenerateKeyArray(Keys);
        const int32 RandIdx = FMath::RandHelper(Keys.Num());
        AddWordToMatrix(Keys[RandIdx]);
    }
}

void AInfinityMatrix::HandleRemoveWordTick()
{
    if (ActiveWords.Num() > 0)
    {
        const int32 RandIdx = FMath::RandHelper(ActiveWords.Num());
        const FString WordToRemove = ActiveWords[RandIdx].Word;
        RemoveWordFromMatrix(WordToRemove);
    }
}

