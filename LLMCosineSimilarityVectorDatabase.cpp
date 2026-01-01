#include "LLMCosineSimilarityVectorDatabase.h"

#include "EmbeddingSimilarity.h"

void ULLMCosineSimilarityVectorDatabase::AddVector(int32 Id, const TArray<float>& Vector)
{
    FFloatArray Wrap; Wrap.Values = Vector;
    VectorStore.Add(Id, MoveTemp(Wrap));
}

bool ULLMCosineSimilarityVectorDatabase::TryGetVector(int32 Id, TArray<float>& OutVector) const
{
    if (const FFloatArray* Found = VectorStore.Find(Id))
    {
        OutVector = Found->Values;
        return true;
    }
    OutVector.Reset();
    return false;
}

void ULLMCosineSimilarityVectorDatabase::QueryNearestNeighbors(const TArray<float>& Query, int32 TopN, TArray<int32>& OutIds) const
{
    OutIds.Reset();
    if (VectorStore.Num() == 0 || TopN <= 0)
    {
        return;
    }

    struct FEntry { int32 Id; float Sim; };
    TArray<FEntry> Entries; Entries.Reserve(VectorStore.Num());
    for (const TPair<int32, FFloatArray>& Pair : VectorStore)
    {
        const float Sim = FEmbeddingSimilarity::ComputeCosineSimilarity(Query, Pair.Value.Values);
        Entries.Add({ Pair.Key, Sim });
    }

    // Sort ascending by similarity to mirror the provided C# sample
    Entries.Sort([](const FEntry& A, const FEntry& B){ return A.Sim < B.Sim; });

    const int32 Count = FMath::Min(TopN, Entries.Num());
    OutIds.Reserve(Count);
    for (int32 i = 0; i < Count; ++i)
    {
        OutIds.Add(Entries[i].Id);
    }
}

void ULLMCosineSimilarityVectorDatabase::GetAllVectors(TArray<int32>& OutIds, TArray<FFloatArray>& OutVectors) const
{
    OutIds.Reset();
    OutVectors.Reset();
    OutIds.Reserve(VectorStore.Num());
    OutVectors.Reserve(VectorStore.Num());
    for (const TPair<int32, FFloatArray>& Pair : VectorStore)
    {
        OutIds.Add(Pair.Key);
        OutVectors.Add(Pair.Value);
    }
}

void ULLMCosineSimilarityVectorDatabase::GetVector_Implementation(int32 Id, TArray<float>& OutVector) const
{
    // Reuse the class's bool-returning helper for convenience
    if (!TryGetVector(Id, OutVector))
    {
        OutVector.Reset();
    }
}
