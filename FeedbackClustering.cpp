#include "FeedbackClustering.h"

TMap<int32, TArray<FEmbedding>> FFeedbackClustering::KMeansClustering(const TArray<FEmbedding>& Embeddings, int32 K)
{
    TMap<int32, TArray<FEmbedding>> Clusters;
    if (K <= 0)
    {
        return Clusters;
    }

    // Initialize empty buckets 0..K-1
    for (int32 i = 0; i < K; ++i)
    {
        Clusters.Add(i, TArray<FEmbedding>());
    }

    for (const FEmbedding& Emb : Embeddings)
    {
        float Sum = 0.f;
        for (float v : Emb)
        {
            Sum += v;
        }

        float Rema = FMath::Fmod(Sum, static_cast<float>(K));
        int32 ClusterIndex = static_cast<int32>(Rema);
        if (ClusterIndex < 0)
        {
            ClusterIndex += K; // keep in [0, K)
        }
        ClusterIndex = FMath::Clamp(ClusterIndex, 0, K - 1);

        Clusters.FindChecked(ClusterIndex).Add(Emb);
    }

    return Clusters;
}

void FFeedbackClustering::FeedbackAdjust(TArray<FEmbedding>& Embeddings, float LearningRate)
{
    for (FEmbedding& Emb : Embeddings)
    {
        for (float& v : Emb)
        {
            v += LearningRate * FMath::Sin(v);
        }
    }
}

