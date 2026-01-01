#include "EmbeddingGenerator.h"

FEmbeddingGenerator::FEmbeddingGenerator(int32 InVocabSize, int32 InEmbeddingSize)
    : VocabSize(InVocabSize)
    , EmbeddingSize(InEmbeddingSize)
{
    EmbeddingMatrix.SetNumZeroed(VocabSize * EmbeddingSize);
    InitializeMatrix();
}

void FEmbeddingGenerator::InitializeMatrix()
{
    for (int32 i = 0; i < VocabSize; ++i)
    {
        for (int32 j = 0; j < EmbeddingSize; ++j)
        {
            const int32 Index = i * EmbeddingSize + j;
            EmbeddingMatrix[Index] = FMath::FRandRange(-1.f, 1.f);
        }
    }
}

TArray<float> FEmbeddingGenerator::GenerateEmbedding(int32 Token) const
{
    TArray<float> Row;
    if (Token < 0 || Token >= VocabSize)
    {
        return Row; // out of range -> empty (behavior guard)
    }

    Row.SetNumUninitialized(EmbeddingSize);
    const int32 Base = Token * EmbeddingSize;
    for (int32 j = 0; j < EmbeddingSize; ++j)
    {
        Row[j] = EmbeddingMatrix[Base + j];
    }
    return Row;
}

void UEmbeddingGenerator::GenerateEmbedding(int32 Token, TArray<float>& OutEmbedding) const
{
    if (!Generator.IsValid() || Generator->GenerateEmbedding(0).Num() != EmbeddingSize)
    {
        Generator = MakeUnique<FEmbeddingGenerator>(FMath::Max(1, VocabSize), FMath::Max(1, EmbeddingSize));
    }
    OutEmbedding = Generator->GenerateEmbedding(Token % FMath::Max(1, VocabSize));
}
