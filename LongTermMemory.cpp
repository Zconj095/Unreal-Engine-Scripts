#include "LongTermMemory.h"

#include "EmbeddingSimilarity.h"

void ULongTermMemory::StoreItem(const FMemoryItem& Item, bool& bStored)
{
    bStored = false;
    if (Item.Key.IsEmpty())
    {
        return;
    }
    if (!MemoryStorage.Contains(Item.Key))
    {
        MemoryStorage.Add(Item.Key, Item);
        bStored = true;
    }
}

bool ULongTermMemory::RetrieveItem(const FString& Key, FMemoryItem& OutItem) const
{
    if (const FMemoryItem* Found = MemoryStorage.Find(Key))
    {
        OutItem = *Found;
        return true;
    }
    return false;
}

void ULongTermMemory::RetrieveRelevantItems(const TArray<float>& QueryEmbedding, int32 TopN, TArray<FMemoryItem>& OutItems) const
{
    OutItems.Reset();
    if (QueryEmbedding.Num() == 0 || TopN <= 0 || MemoryStorage.Num() == 0)
    {
        return;
    }

    struct FScored
    {
        float Score;
        const FMemoryItem* Item;
    };

    TArray<FScored> Scored; Scored.Reserve(MemoryStorage.Num());
    for (const TPair<FString, FMemoryItem>& Pair : MemoryStorage)
    {
        const float Sim = FEmbeddingSimilarity::ComputeCosineSimilarity(QueryEmbedding, Pair.Value.Embedding);
        Scored.Add({ Sim, &Pair.Value });
    }

    Scored.Sort([](const FScored& A, const FScored& B){ return A.Score > B.Score; });

    const int32 Count = FMath::Min(TopN, Scored.Num());
    OutItems.Reserve(Count);
    for (int32 i = 0; i < Count; ++i)
    {
        OutItems.Add(*Scored[i].Item);
    }
}

