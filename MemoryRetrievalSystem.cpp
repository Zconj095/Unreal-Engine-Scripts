#include "MemoryRetrievalSystem.h"

void UMemoryRetrievalSystem::RetrieveRelevantVectors_Implementation(const TArray<float>& /*InputEmbedding*/, UMultiDatabaseManager* /*DbManager*/, int32 /*TopN*/, TArray<FRetrievalResult>& OutResults) const
{
    OutResults.Reset();
}

