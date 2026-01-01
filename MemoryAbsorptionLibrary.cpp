#include "MemoryAbsorptionLibrary.h"

#include "LLMCosineSimilarityVectorDatabase.h"
#include "LongTermMemory.h"

void UMemoryAbsorptionLibrary::AbsorbDatabase(ULLMCosineSimilarityVectorDatabase* VectorDB, ULongTermMemory* LongTermMem, int32& OutStoredCount)
{
    OutStoredCount = 0;
    if (!VectorDB || !LongTermMem)
    {
        return;
    }

    TArray<int32> Ids; TArray<FFloatArray> Vectors;
    VectorDB->GetAllVectors(Ids, Vectors);
    const int32 N = FMath::Min(Ids.Num(), Vectors.Num());
    for (int32 i = 0; i < N; ++i)
    {
        const int32 Id = Ids[i];
        const TArray<float>& Vec = Vectors[i].Values;

        FMemoryItem Item;
        Item.Key = LexToString(Id);
        Item.Content = FString::Printf(TEXT("Absorbed from database: %d"), Id);
        Item.Embedding = Vec;

        bool bStored = false;
        LongTermMem->StoreItem(Item, bStored);
        if (bStored)
        {
            ++OutStoredCount;
        }
    }
}

