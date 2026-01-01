#include "AutoGenerationPipeline.h"

void UMemoryRetrievalSystem2Base::RetrieveMemory_Implementation(const TArray<float>& /*InputEmbedding*/, TArray<FMemoryEmbedding>& OutMemories) const
{
    OutMemories.Reset();
}

FString UAutoGenerationPipeline::Generate(const FString& /*UserInput*/, const TArray<float>& InputEmbedding) const
{
    if (!MemoryRetrievalSystem || !OutputGenerator)
    {
        return TEXT("AutoGenerationPipeline not configured");
    }

    // Step 1: Retrieve relevant memories
    TArray<FMemoryEmbedding> RelevantMemories;
    MemoryRetrievalSystem->RetrieveMemory(InputEmbedding, RelevantMemories);

    // Step 2/3: Generate output using context and input
    return OutputGenerator->GenerateOutput(InputEmbedding, RelevantMemories);
}

