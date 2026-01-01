#include "MemoryRecollectionManager.h"

#include "EmbeddingSimilarity.h"

void UMemoryRecollectionManager::Initialize(ULayeredShortTermMemory* InSTM, ULongTermMemory* InLTM)
{
    STM = InSTM;
    LTM = InLTM;
}

void UMemoryRecollectionManager::AddToMemory(const FMemoryItem& MemoryItem)
{
    if (!STM || !LTM)
    {
        return;
    }
    if (MemoryItem.RelevanceScore >= 0.7f)
    {
        STM->AddItem(MemoryItem);
    }
    else
    {
        bool bStored = false;
        LTM->StoreItem(MemoryItem, bStored);
    }
}

void UMemoryRecollectionManager::Recollect(const TArray<float>& QueryEmbedding, int32 TopN, TArray<FMemoryItem>& OutResults) const
{
    OutResults.Reset();
    if (!STM || !LTM || QueryEmbedding.Num() == 0 || TopN <= 0)
    {
        return;
    }

    // Gather STM items from layered caches (L1,L2,L3)
    TArray<FMemoryItem> StmItems;
    StmItems.Reserve(STM->GetL1().Num() + STM->GetL2().Num() + STM->GetL3().Num());
    StmItems.Append(STM->GetL1());
    StmItems.Append(STM->GetL2());
    StmItems.Append(STM->GetL3());

    // Sort STM items by recency (Timestamp descending)
    StmItems.Sort([](const FMemoryItem& A, const FMemoryItem& B){ return A.Timestamp > B.Timestamp; });
    if (StmItems.Num() > TopN)
    {
        StmItems.SetNum(TopN);
    }

    // Get relevant items from LTM using cosine similarity
    TArray<FMemoryItem> LtmItems;
    LTM->RetrieveRelevantItems(QueryEmbedding, TopN, LtmItems);

    // Combine and deduplicate by Key
    TMap<FString, FMemoryItem> CombinedMap;
    for (const FMemoryItem& It : StmItems) { CombinedMap.FindOrAdd(It.Key, It); }
    for (const FMemoryItem& It : LtmItems) { CombinedMap.FindOrAdd(It.Key, It); }

    TArray<FMemoryItem> CombinedItems; CombinedItems.Reserve(CombinedMap.Num());
    for (const TPair<FString, FMemoryItem>& P : CombinedMap) CombinedItems.Add(P.Value);

    // Rank by combined relevance (0.8 * similarity + 0.2 * recency)
    CombinedItems.Sort([&QueryEmbedding](const FMemoryItem& A, const FMemoryItem& B)
    {
        return ComputeRelevance(QueryEmbedding, A) > ComputeRelevance(QueryEmbedding, B);
    });

    const int32 Count = FMath::Min(TopN, CombinedItems.Num());
    OutResults.Append(CombinedItems.GetData(), Count);
}

TOptional<FMemoryItem> UMemoryRecollectionManager::GetMemoryById(const FString& Id) const
{
    if (Id.IsEmpty() || !STM || !LTM)
    {
        return TOptional<FMemoryItem>();
    }

    FMemoryItem Found;
    if (STM->RetrieveItem(Id, Found))
    {
        return TOptional<FMemoryItem>(Found);
    }
    if (LTM->RetrieveItem(Id, Found))
    {
        return TOptional<FMemoryItem>(Found);
    }
    return TOptional<FMemoryItem>();
}

float UMemoryRecollectionManager::ComputeRelevance(const TArray<float>& Query, const FMemoryItem& Item)
{
    const float Similarity = FEmbeddingSimilarity::ComputeCosineSimilarity(Query, Item.Embedding);
    const float Recency = ComputeRecencyFactor(Item.Timestamp);
    return 0.8f * Similarity + 0.2f * Recency;
}

float UMemoryRecollectionManager::ComputeRecencyFactor(const FDateTime& TimestampUtc)
{
    const FTimespan Delta = FDateTime::UtcNow() - TimestampUtc;
    const double Seconds = Delta.GetTotalSeconds();
    return static_cast<float>(1.0 / (1.0 + FMath::Max(0.0, Seconds)));
}

