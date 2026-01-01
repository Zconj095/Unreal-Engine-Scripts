#include "ConversationalAIManager.h"

#include "Misc/Crc.h"

void UMemoryRecollectionBase::Recollect_Implementation(const TArray<float>& /*InputEmbedding*/, TArray<FMemoryItem>& OutMemories) const
{
    OutMemories.Reset();
}

void UMultiInstructionParserBase::ParseInstructionSequences_Implementation(const FString& /*Input*/, TArray<FStringArray>& OutSequences) const
{
    OutSequences.Reset();
}

FString USynchronousExecutionManagerBase::ExecuteInstructionSequences_Implementation(const TArray<FStringArray>& /*InstructionSequences*/) const
{
    return FString();
}

FString UConversationalAIManager::HandleUserInput(const FString& UserInput)
{
    if (!ContextManager)
    {
        return TEXT("ConversationalAIManager not configured (ContextManager missing)");
    }

    // Step 1: Absorb user input into STM
    FMemoryItem Item; Item.Content = UserInput; // name/metadata omitted in UE version
    ContextManager->AddToContext(Item);

    // Step 2: Analyze input for instructions
    TArray<FStringArray> InstructionSequences;
    if (InstructionParser)
    {
        InstructionParser->ParseInstructionSequences(UserInput, InstructionSequences);
    }

    // Step 3: Execute instructions (if any)
    FString ExecResult;
    if (InstructionSequences.Num() > 0 && InstructionManager)
    {
        ExecResult = InstructionManager->ExecuteInstructionSequences(InstructionSequences);
    }

    // Step 4: Retrieve relevant memories
    TArray<float> InputEmbedding;
    GenerateEmbedding(UserInput, InputEmbedding);

    TArray<FMemoryItem> RelevantMemories;
    if (MemoryRecollection)
    {
        MemoryRecollection->Recollect(InputEmbedding, RelevantMemories);
    }

    // Step 5: Generate response
    TArray<FString> Lines;
    Lines.Reserve(RelevantMemories.Num());
    for (const FMemoryItem& M : RelevantMemories)
    {
        Lines.Add(M.Content);
    }
    const FString MemoryContext = FString::Join(Lines, TEXT("\n"));
    const FString InstructionContext = ExecResult;

    return FString::Printf(TEXT("Instruction Results:\n%s\n\nMemory Context:\n%s\n\nGenerative Reply:\nHello! Let's continue our conversation!"), *InstructionContext, *MemoryContext);
}

void UConversationalAIManager::GenerateEmbedding(const FString& Input, TArray<float>& OutEmbedding)
{
    TArray<FString> Words;
    Input.ParseIntoArrayWS(Words);
    OutEmbedding.SetNumUninitialized(Words.Num());
    constexpr float Div = 2147483647.0f; // int32 max
    for (int32 i = 0; i < Words.Num(); ++i)
    {
        const int32 Hash = static_cast<int32>(FCrc::StrCrc32(*Words[i]));
        // Map to [-1,1] roughly like C# hash/int.MaxValue
        OutEmbedding[i] = static_cast<float>(Hash) / Div;
    }
}
