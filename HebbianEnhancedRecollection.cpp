#include "HebbianEnhancedRecollection.h"

TArray<FMemoryItem> FHebbianEnhancedRecollection::Recollect(const TArray<float>& QueryEmbedding, int32 TopN) const
{
    TArray<FMemoryItem> Empty;
    if (!MemoryRecollection || !HebbianNetwork || TopN <= 0)
    {
        return Empty;
    }

    // Initial results
    TArray<FMemoryItem> InitialResults;
    MemoryRecollection->Recollect(QueryEmbedding, TopN, InitialResults);

    // Fetch associated memories from Hebbian connections
    TSet<int32> SeenAssocIds;
    TArray<FMemoryItem> Associated;
    for (const FMemoryItem& Item : InitialResults)
    {
        const int32 NodeId = static_cast<int32>(GetTypeHash(Item.Key));
        const TArray<int32> AssocIds = HebbianNetwork->GetAssociatedMemories(NodeId);
        for (int32 AssocId : AssocIds)
        {
            if (SeenAssocIds.Contains(AssocId))
            {
                continue;
            }
            SeenAssocIds.Add(AssocId);
            const FString IdStr = FString::FromInt(AssocId);
            if (TOptional<FMemoryItem> Found = MemoryRecollection->GetMemoryById(IdStr))
            {
                Associated.Add(MoveTemp(Found.GetValue()));
                if (Associated.Num() >= TopN)
                {
                    break;
                }
            }
        }
        if (Associated.Num() >= TopN)
        {
            break;
        }
    }

    // Merge and dedupe by Key
    TArray<FMemoryItem> Combined;
    Combined.Reserve(FMath::Min(TopN, InitialResults.Num() + Associated.Num()));
    TSet<FString> SeenKeys;
    for (const FMemoryItem& It : InitialResults)
    {
        if (!SeenKeys.Contains(It.Key))
        {
            SeenKeys.Add(It.Key);
            Combined.Add(It);
            if (Combined.Num() >= TopN) return Combined;
        }
    }
    for (const FMemoryItem& It : Associated)
    {
        if (!SeenKeys.Contains(It.Key))
        {
            SeenKeys.Add(It.Key);
            Combined.Add(It);
            if (Combined.Num() >= TopN) return Combined;
        }
    }

    return Combined;
}

