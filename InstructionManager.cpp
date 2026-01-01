#include "InstructionManager.h"

void UInstructionManager::AddInstruction(const FInstruction& Instruction)
{
    Instructions.Add(Instruction);
    SortByPriority();
}

void UInstructionManager::AddInstructions(const TArray<FInstruction>& NewInstructions)
{
    Instructions.Append(NewInstructions);
    SortByPriority();
}

FString UInstructionManager::GetResponse(const FString& Input) const
{
    for (const FInstruction& I : Instructions)
    {
        if (Input.Contains(I.Trigger)) // case-sensitive like C# default
        {
            return I.ResponseTemplate;
        }
    }
    return TEXT("No matching instruction found.");
}

void UInstructionManager::SortByPriority()
{
    Instructions.Sort([](const FInstruction& A, const FInstruction& B){ return A.Priority < B.Priority; });
}

