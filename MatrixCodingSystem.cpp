#include "MatrixCodingSystem.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixCodingSystem::AMatrixCodingSystem()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixCodingSystem::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Coding System Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CreateCodeTimerHandle,
            this,
            &AMatrixCodingSystem::HandleCreateCodeTick,
            FMath::Max(0.0f, CreateCodeInterval),
            true);

        World->GetTimerManager().SetTimer(
            DebugCodeTimerHandle,
            this,
            &AMatrixCodingSystem::HandleDebugCodeTick,
            FMath::Max(0.0f, DebugCodeInterval),
            true);

        World->GetTimerManager().SetTimer(
            AnalyzeCodeTimerHandle,
            this,
            &AMatrixCodingSystem::HandleAnalyzeCodeTick,
            FMath::Max(0.0f, AnalyzeCodeInterval),
            true);
    }
}

void AMatrixCodingSystem::CreateCode(const FString& Content)
{
    FMatrixCode Code(Content);
    UE_LOG(LogTemp, Log, TEXT("New code created - ID: %s, Has Errors: %s"), *Code.CodeID, Code.bHasErrors ? TEXT("true") : TEXT("false"));

    if (Code.bHasErrors)
    {
        DebuggingQueue.Add(Code);
        UE_LOG(LogTemp, Warning, TEXT("Code sent to debugging queue - ID: %s"), *Code.CodeID);
    }
    else
    {
        SendToCompiler(Code);
    }
}

void AMatrixCodingSystem::DebugCode()
{
    if (DebuggingQueue.Num() > 0)
    {
        FMatrixCode Code = DebuggingQueue[0];
        DebuggingQueue.RemoveAt(0);

        const FString Cleaned = Code.Content.Replace(TEXT("BUG"), TEXT(""));
        Code = FMatrixCode(Cleaned); // simulate debugging by creating a new code instance
        UE_LOG(LogTemp, Log, TEXT("Debugged code - ID: %s, Has Errors: %s"), *Code.CodeID, Code.bHasErrors ? TEXT("true") : TEXT("false"));

        if (!Code.bHasErrors)
        {
            SendToCompiler(Code);
        }
        else
        {
            DebuggingQueue.Add(Code);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No code in the debugging queue."));
    }
}

void AMatrixCodingSystem::SendToCompiler(const FMatrixCode& Code)
{
    CompiledCode.Add(Code);
    UE_LOG(LogTemp, Log, TEXT("Code sent to compiler - ID: %s"), *Code.CodeID);
}

void AMatrixCodingSystem::AnalyzeCompiledCode()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing all compiled code..."));
    for (const FMatrixCode& Code : CompiledCode)
    {
        UE_LOG(LogTemp, Log, TEXT("Compiled Code - ID: %s, Content: %s"), *Code.CodeID, *Code.Content);
    }
}

// Timers
void AMatrixCodingSystem::HandleCreateCodeTick()
{
    const bool bClean = FMath::FRand() > 0.5f;
    const FString Content = bClean ? TEXT("Example code without BUG") : TEXT("Example code with BUG");
    CreateCode(Content);
}

void AMatrixCodingSystem::HandleDebugCodeTick()
{
    DebugCode();
}

void AMatrixCodingSystem::HandleAnalyzeCodeTick()
{
    AnalyzeCompiledCode();
}

