#include "MatrixAnalyzer.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixAnalyzer::AMatrixAnalyzer()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixAnalyzer::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Analyzer Initialized."));

    // Bind admin notification
    OnErrorDetected.AddDynamic(this, &AMatrixAnalyzer::NotifyAdmin);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AnalyzeCodeTimerHandle,
            this,
            &AMatrixAnalyzer::HandleAnalyzeCodeTick,
            FMath::Max(0.0f, AnalyzeCodeInterval),
            true);

        World->GetTimerManager().SetTimer(
            HandleTaskTimerHandle,
            this,
            &AMatrixAnalyzer::HandleHandleTaskTick,
            FMath::Max(0.0f, HandleTaskInterval),
            true);
    }
}

void AMatrixAnalyzer::AnalyzeCode(const FString& CodeSection)
{
    FString ErrorDetails;
    const bool bHasErrors = DetectErrors(CodeSection, ErrorDetails);

    // Log and store result
    AnalysisResults.Emplace(CodeSection, bHasErrors, ErrorDetails);

    UE_LOG(LogTemp, Log, TEXT("Analyzed Code Section: %s, Errors Found: %s"), *CodeSection, bHasErrors ? TEXT("true") : TEXT("false"));

    if (bHasErrors)
    {
        UE_LOG(LogTemp, Warning, TEXT("Errors Detected in Code Section: %s"), *CodeSection);
        OnErrorDetected.Broadcast(FString::Printf(TEXT("Errors Detected: %s"), *ErrorDetails));
    }
    else
    {
        CombineKnowledge(CodeSection);
    }
}

bool AMatrixAnalyzer::DetectErrors(const FString& Code, FString& OutErrorDetails) const
{
    if (Code.Contains(TEXT("BUG")))
    {
        OutErrorDetails = TEXT("Syntax error at line 42.");
        return true;
    }
    OutErrorDetails.Reset();
    return false;
}

void AMatrixAnalyzer::CombineKnowledge(const FString& CompiledCode)
{
    KnowledgeDatabase.Add(CompiledCode);
    const FString Wisdom = FString::Printf(TEXT("Wisdom Derived from %s"), *CompiledCode);
    WisdomDatabase.Add(Wisdom);

    UE_LOG(LogTemp, Log, TEXT("Knowledge combined. Total Knowledge Entries: %d"), KnowledgeDatabase.Num());
    UE_LOG(LogTemp, Log, TEXT("Wisdom Generated: %s"), *Wisdom);
}

void AMatrixAnalyzer::HandleTask(const FString& Task)
{
    UE_LOG(LogTemp, Log, TEXT("Matrix Analyzer handling task: %s"), *Task);
    if (WisdomDatabase.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Wisdom Applied: %s"), *WisdomDatabase.Last());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No wisdom available for task."));
    }
}

void AMatrixAnalyzer::NotifyAdmin(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[HeadAdmin Notification] %s"), *Message);
}

// Timers
void AMatrixAnalyzer::HandleAnalyzeCodeTick()
{
    AnalyzeCode(TEXT("SampleCodeWithoutErrors"));
    AnalyzeCode(TEXT("SampleCodeWithBUG"));
}

void AMatrixAnalyzer::HandleHandleTaskTick()
{
    HandleTask(TEXT("Optimize Matrix System"));
}

