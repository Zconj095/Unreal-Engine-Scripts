#include "EmbeddingSimilarity.h"

float FEmbeddingSimilarity::ComputeCosineSimilarity(const FEmbedding& A, const FEmbedding& B)
{
    const int32 N = A.Num();
    if (N == 0 || N != B.Num())
    {
        return 0.f;
    }

    float Dot = 0.f;
    float MagA = 0.f;
    float MagB = 0.f;
    for (int32 i = 0; i < N; ++i)
    {
        const float Va = A[i];
        const float Vb = B[i];
        Dot += Va * Vb;
        MagA += Va * Va;
        MagB += Vb * Vb;
    }

    const float Den = FMath::Sqrt(MagA) * FMath::Sqrt(MagB);
    if (Den <= KINDA_SMALL_NUMBER)
    {
        return 0.f;
    }
    return Dot / Den;
}

TArray<TArray<FEmbedding>> FEmbeddingSimilarity::CategorizeData(const TArray<FEmbedding>& Embeddings, float SimilarityThreshold)
{
    TArray<TArray<FEmbedding>> Categories;
    Categories.Reserve(Embeddings.Num());

    for (const FEmbedding& Emb : Embeddings)
    {
        bool bAdded = false;
        for (TArray<FEmbedding>& Category : Categories)
        {
            if (Category.Num() > 0)
            {
                const float Sim = ComputeCosineSimilarity(Emb, Category[0]);
                if (Sim >= SimilarityThreshold)
                {
                    Category.Add(Emb);
                    bAdded = true;
                    break;
                }
            }
        }

        if (!bAdded)
        {
            TArray<FEmbedding> NewCat;
            NewCat.Add(Emb);
            Categories.Add(MoveTemp(NewCat));
        }
    }

    return Categories;
}

