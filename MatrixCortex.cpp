#include "MatrixCortex.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixCortex::AMatrixCortex()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixCortex::BeginPlay()
{
    Super::BeginPlay();

    InitializeCortex();

    // Initial setup
    AddSkillToCortex(TEXT("Programming"), TEXT("Cognitive"));
    AddSkillToCortex(TEXT("Athletics"), TEXT("Physical"));
    AddSkillToCortex(TEXT("Problem Solving"), TEXT("Cognitive"));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddSkillTimerHandle,
            this,
            &AMatrixCortex::HandleAddSkillTick,
            FMath::Max(0.0f, AddSkillInterval),
            true);

        World->GetTimerManager().SetTimer(
            ImproveSkillTimerHandle,
            this,
            &AMatrixCortex::HandleImproveSkillTick,
            FMath::Max(0.0f, ImproveSkillInterval),
            true);

        World->GetTimerManager().SetTimer(
            RecycleMemoryTimerHandle,
            this,
            &AMatrixCortex::HandleRecycleMemoryTick,
            FMath::Max(0.0f, RecycleMemoryInterval),
            true);

        World->GetTimerManager().SetTimer(
            DisplaySkillsTimerHandle,
            this,
            &AMatrixCortex::HandleDisplaySkillsTick,
            FMath::Max(0.0f, DisplaySkillsInterval),
            true);
    }
}

void AMatrixCortex::InitializeCortex()
{
    MasterDatabase = MakeUnique<FMatrixDatabase>(6000.f); // Large initial capacity
    UE_LOG(LogTemp, Log, TEXT("Matrix Cortex initialized with Master Database."));
}

void AMatrixCortex::AddSkillToCortex(const FString& SkillName, const FString& Category)
{
    if (!MasterDatabase)
    {
        InitializeCortex();
    }

    // Simulated memory usage per skill is 0.1f
    if (MasterDatabase->MemoryUsage + 0.1f > MasterDatabase->MemoryCapacity)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot add skill '%s'. Memory limit reached. Recycle memory first."), *SkillName);
        return;
    }

    MasterDatabase->AddSkill(FBrainSkill(SkillName, Category));
}

void AMatrixCortex::ImproveSkill(const FString& SkillName, int32 Increment)
{
    if (!MasterDatabase)
    {
        UE_LOG(LogTemp, Warning, TEXT("Master Database not initialized."));
        return;
    }

    for (FBrainSkill& Skill : MasterDatabase->Skills)
    {
        if (Skill.SkillName == SkillName)
        {
            Skill.ImproveSkill(Increment);
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Skill '%s' not found in the Matrix Cortex."), *SkillName);
}

void AMatrixCortex::RecycleCortexMemory()
{
    if (MasterDatabase)
    {
        MasterDatabase->RecycleMemory();
    }
}

void AMatrixCortex::DisplaySkills() const
{
    if (!MasterDatabase)
    {
        UE_LOG(LogTemp, Warning, TEXT("Master Database not initialized."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Displaying all skills in the Matrix Cortex:"));
    for (const FBrainSkill& Skill : MasterDatabase->Skills)
    {
        UE_LOG(LogTemp, Log, TEXT("Skill: %s, Level: %d, Category: %s"), *Skill.SkillName, Skill.Level, *Skill.Category);
    }
}

// Timers
void AMatrixCortex::HandleAddSkillTick()
{
    static const TArray<FString> Categories = { TEXT("Cognitive"), TEXT("Physical"), TEXT("Creative") };
    const int32 Index = FMath::RandRange(0, Categories.Num() - 1);
    const FString SkillName = FString::Printf(TEXT("Skill_%d"), MasterDatabase ? MasterDatabase->Skills.Num() + 1 : 1);
    AddSkillToCortex(SkillName, Categories[Index]);
}

void AMatrixCortex::HandleImproveSkillTick()
{
    if (MasterDatabase && MasterDatabase->Skills.Num() > 0)
    {
        const int32 Index = FMath::RandRange(0, MasterDatabase->Skills.Num() - 1);
        const int32 Increment = FMath::RandRange(1, 4); // 1..4 inclusive
        ImproveSkill(MasterDatabase->Skills[Index].SkillName, Increment);
    }
}

void AMatrixCortex::HandleRecycleMemoryTick()
{
    RecycleCortexMemory();
}

void AMatrixCortex::HandleDisplaySkillsTick() const
{
    DisplaySkills();
}

