#include "ContextManager.h"

#include "Misc/Crc.h"

void UContextManager::Initialize(int32 InMaxTokens)
{
    MaxTokens = FMath::Max(0, InMaxTokens);
}

void UContextManager::AddToContext(const FMemoryItem& MemoryItem)
{
    TArray<int32> NewTokens;
    Tokenize(MemoryItem.Content, NewTokens);
    const int32 NewTokenCount = NewTokens.Num();

    while (GetContextTokenCount() + NewTokenCount > MaxTokens && ContextBuffer.Num() > 0)
    {
        ContextBuffer.RemoveAt(0);
    }

    ContextBuffer.Add(MemoryItem);
}

FString UContextManager::GetContextAsString() const
{
    TArray<FString> Parts;
    Parts.Reserve(ContextBuffer.Num());
    for (const FMemoryItem& Item : ContextBuffer)
    {
        Parts.Add(Item.Content);
    }
    return FString::Join(Parts, TEXT(" "));
}

int32 UContextManager::GetContextTokenCount() const
{
    int32 Total = 0;
    TArray<int32> Tokens;
    for (const FMemoryItem& Item : ContextBuffer)
    {
        Tokens.Reset();
        Tokenize(Item.Content, Tokens);
        Total += Tokens.Num();
    }
    return Total;
}

void UContextManager::Tokenize(const FString& Input, TArray<int32>& OutTokenHashes)
{
    TArray<FString> Words;
    Input.ParseIntoArrayWS(Words);
    OutTokenHashes.SetNumUninitialized(Words.Num());
    for (int32 i = 0; i < Words.Num(); ++i)
    {
        OutTokenHashes[i] = static_cast<int32>(FCrc::StrCrc32(*Words[i]));
    }
}

