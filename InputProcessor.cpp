#include "InputProcessor.h"

void UInputProcessor::ProcessInput(const FString& Input, TArray<float>& OutEmbedding) const
{
    OutEmbedding.Reset();

    if (!Tokenizer || !EmbeddingGenerator)
    {
        return;
    }

    TArray<int32> Tokens;
    Tokenizer->Tokenize(Input, Tokens);

    const int32 EmbSize = FMath::Max(1, EmbeddingGenerator->EmbeddingSize);
    if (Tokens.Num() > 0)
    {
        OutEmbedding.Reserve(Tokens.Num() * EmbSize);
    }

    TArray<float> TokenEmbedding;
    for (const int32 Token : Tokens)
    {
        TokenEmbedding.Reset();
        EmbeddingGenerator->GenerateEmbedding(Token, TokenEmbedding);
        OutEmbedding.Append(TokenEmbedding);
    }
}

