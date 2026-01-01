#include "DialogueGenerator.h"

FString FDialogueGenerator::Generate(const FString& UserInput,
                                     const TArray<FMemoryItem>& RelevantMemories,
                                     const FString& InstructionResult)
{
    TArray<FString> MemoryLines;
    MemoryLines.Reserve(RelevantMemories.Num());
    for (const FMemoryItem& Item : RelevantMemories)
    {
        MemoryLines.Add(Item.Content);
    }

    const FString MemoryContent = FString::Join(MemoryLines, TEXT("\n"));
    const FString InstructionContent = InstructionResult; // already a string, mirror C# ToString behavior

    FString Output;
    Output += TEXT("I recall the following:\n");
    Output += MemoryContent;
    Output += TEXT("\n\nBased on your instruction:\n");
    Output += InstructionContent;
    Output += TEXT("\n\nLet's continue the discussion!");

    return Output;
}

