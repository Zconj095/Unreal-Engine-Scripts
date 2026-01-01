#include "EmbeddingCompressor.h"

TArray<uint8> FEmbeddingCompressor::CompressEmbedding(const TArray<float>& Embedding)
{
    TArray<uint8> Out;
    Out.Reserve(Embedding.Num());
    for (float V : Embedding)
    {
        const float Clamped = FMath::Clamp(V, 0.f, 1.f);
        Out.Add(static_cast<uint8>(Clamped * 255.f)); // truncates toward zero like C# cast
    }
    return Out;
}

TArray<float> FEmbeddingCompressor::DecompressEmbedding(const TArray<uint8>& CompressedEmbedding)
{
    TArray<float> Out;
    Out.Reserve(CompressedEmbedding.Num());
    for (uint8 B : CompressedEmbedding)
    {
        Out.Add(static_cast<float>(B) / 255.f);
    }
    return Out;
}

