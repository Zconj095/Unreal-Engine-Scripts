#include "InstructionSetManager.h"

void UInstructionSetManager::SetDefaultInstructionSet(const FInstructionSet& InstructionSet)
{
    DefaultInstructionSet = InstructionSet;
}

FInstructionSet UInstructionSetManager::GetDefaultInstructionSet() const
{
    return DefaultInstructionSet;
}

