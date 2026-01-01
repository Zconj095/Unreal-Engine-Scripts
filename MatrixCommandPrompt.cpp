#include "MatrixCommandPrompt.h"

#include "Engine/World.h"
#include "TimerManager.h"

AMatrixCommandPrompt::AMatrixCommandPrompt()
{
    PrimaryActorTick.bCanEverTick = false; // Use timers instead of Tick
}

void AMatrixCommandPrompt::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Matrix Command Prompt Initialized."));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AddCommandTimerHandle,
            this,
            &AMatrixCommandPrompt::HandleAddCommandTick,
            FMath::Max(0.0f, AddCommandInterval),
            true);

        World->GetTimerManager().SetTimer(
            ExecuteCommandTimerHandle,
            this,
            &AMatrixCommandPrompt::HandleExecuteCommandTick,
            FMath::Max(0.0f, ExecuteCommandInterval),
            true);

        World->GetTimerManager().SetTimer(
            ReplenishEnergyTimerHandle,
            this,
            &AMatrixCommandPrompt::HandleReplenishEnergyTick,
            FMath::Max(0.0f, ReplenishEnergyInterval),
            true);
    }
}

void AMatrixCommandPrompt::AddCommand(const FString& CommandText, float RequiredEnergy)
{
    if (RequiredEnergy > AvailableEnergy)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough energy to execute the command."));
        return;
    }

    const FMatrixCommand Cmd(CommandText, RequiredEnergy);
    CommandQueue.Add(Cmd);

    UE_LOG(LogTemp, Log, TEXT("Command added - ID: %s, Text: %s, Required Energy: %.2f"), *Cmd.CommandID, *CommandText, RequiredEnergy);
}

void AMatrixCommandPrompt::ExecuteCommand()
{
    if (CommandQueue.Num() > 0)
    {
        const FMatrixCommand Cmd = CommandQueue[0];
        CommandQueue.RemoveAt(0);

        AvailableEnergy -= Cmd.RequiredEnergy;
        UE_LOG(LogTemp, Log, TEXT("Executed Command - ID: %s, Text: %s, Remaining Energy: %.2f"), *Cmd.CommandID, *Cmd.CommandText, AvailableEnergy);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No commands in the queue to execute."));
    }
}

void AMatrixCommandPrompt::ReplenishEnergy(float EnergyAmount)
{
    AvailableEnergy += EnergyAmount;
    UE_LOG(LogTemp, Log, TEXT("Energy replenished by %.2f. Available Energy: %.2f"), EnergyAmount, AvailableEnergy);
}

// Timers
void AMatrixCommandPrompt::HandleAddCommandTick()
{
    const float RequiredEnergy = FMath::FRandRange(10.f, 50.f);
    AddCommand(FString::Printf(TEXT("Automated Command - Energy %.2f"), RequiredEnergy), RequiredEnergy);
}

void AMatrixCommandPrompt::HandleExecuteCommandTick()
{
    ExecuteCommand();
}

void AMatrixCommandPrompt::HandleReplenishEnergyTick()
{
    const float EnergyAmount = FMath::FRandRange(20.f, 40.f);
    ReplenishEnergy(EnergyAmount);
}

