#include "ConversationalInstructionHandler.h"

FString UConversationalInstructionHandler::ExecuteFromInput(const FString& UserInput, bool& bExecuted) const
{
    bExecuted = false;

    if (!InstructionParser || !ExecutionManager)
    {
        return FString();
    }

    TArray<FStringArray> InstructionSequences;
    InstructionParser->ParseInstructionSequences(UserInput, InstructionSequences);

    if (InstructionSequences.Num() > 0)
    {
        bExecuted = true;
        return ExecutionManager->ExecuteInstructionSequences(InstructionSequences);
    }

    return FString(); // No instructions to execute
}
