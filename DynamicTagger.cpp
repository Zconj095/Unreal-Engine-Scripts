#include "DynamicTagger.h"

FDynamicTagger::FDynamicTagger(int32 InClusterCount)
    : ClusterCount(InClusterCount)
{
}

void FDynamicTagger::AddData(const FEmbedding& Embedding)
{
    DataEmbeddings.Add(Embedding);
}

TMap<int32, TArray<FEmbedding>> FDynamicTagger::ClusterData() const
{
    // Placeholder: mirror C# example behavior
    TMap<int32, TArray<FEmbedding>> Clusters;
    if (DataEmbeddings.Num() > 0)
    {
        TArray<FEmbedding> Bucket;
        Bucket.Add(DataEmbeddings[0]);
        Clusters.Add(0, MoveTemp(Bucket));
    }
    return Clusters;
}

TArray<FString> FDynamicTagger::GenerateTags(const TMap<int32, TArray<FEmbedding>>& Clusters) const
{
    TArray<FString> Tags;
    Tags.Reserve(Clusters.Num());
    for (const TPair<int32, TArray<FEmbedding>>& Pair : Clusters)
    {
        Tags.Add(FString::Printf(TEXT("Cluster-%d"), Pair.Key));
    }
    return Tags;
}

