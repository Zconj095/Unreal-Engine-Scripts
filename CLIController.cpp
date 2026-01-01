#include "CLIController.h"

void UCLIController::RegisterCommand(FName Command, FCLICommand Action)
{
    Commands.Add(Command, Action);
}

void UCLIController::UnregisterCommand(FName Command)
{
    Commands.Remove(Command);
}

bool UCLIController::ExecuteCommand(const FString& Input)
{
    TArray<FString> Parts;
    Input.ParseIntoArrayWS(Parts);

    if (Parts.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No input provided."));
        return false;
    }

    const FName Cmd(*Parts[0]);
    FString Args;
    if (Parts.Num() > 1)
    {
        TArray<FString> ArgParts;
        ArgParts.Reserve(Parts.Num() - 1);
        for (int32 i = 1; i < Parts.Num(); ++i)
        {
            ArgParts.Add(Parts[i]);
        }
        Args = FString::Join(ArgParts, TEXT(" "));
    }

    if (FCLICommand* Found = Commands.Find(Cmd))
    {
        if (Found->IsBound())
        {
            Found->Execute(Args);
            return true;
        }
        UE_LOG(LogTemp, Warning, TEXT("Command '%s' is not bound."), *Cmd.ToString());
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("Unknown command: %s"), *Cmd.ToString());
    return false;
}

void UCLIController::GetRegisteredCommands(TArray<FName>& OutCommands) const
{
    Commands.GetKeys(OutCommands);
}
