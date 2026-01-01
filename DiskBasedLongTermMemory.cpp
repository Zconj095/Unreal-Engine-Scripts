#include "DiskBasedLongTermMemory.h"

#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

FDiskBasedLongTermMemory::FDiskBasedLongTermMemory(const FString& InPath)
    : StoragePath(InPath)
{
    EnsureDirectory(StoragePath);
}

bool FDiskBasedLongTermMemory::EnsureDirectory(const FString& Path)
{
    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    return PF.CreateDirectoryTree(*Path);
}

FString FDiskBasedLongTermMemory::SerializeEmbedding(const TArray<float>& Embedding)
{
    TArray<FString> Parts;
    Parts.Reserve(Embedding.Num());
    for (float V : Embedding)
    {
        Parts.Add(LexToString(V));
    }
    return FString::Join(Parts, TEXT(","));
}

bool FDiskBasedLongTermMemory::ParseEmbedding(const FString& Line, TArray<float>& OutEmbedding)
{
    OutEmbedding.Reset();
    if (Line.IsEmpty())
    {
        return true; // allow empty embedding
    }

    TArray<FString> Tokens;
    Line.ParseIntoArray(Tokens, TEXT(","), /*CullEmpty*/false);
    OutEmbedding.Reserve(Tokens.Num());
    for (const FString& Tok : Tokens)
    {
        float Value = 0.f;
        if (!LexTryParseString(Value, *Tok))
        {
            return false;
        }
        OutEmbedding.Add(Value);
    }
    return true;
}

bool FDiskBasedLongTermMemory::AddItem(const FMemoryItem& Item) const
{
    if (!EnsureDirectory(StoragePath))
    {
        return false;
    }

    const FString FilePath = FPaths::Combine(StoragePath, Item.Key + TEXT(".txt"));

    const FString IsoTs = Item.Timestamp.ToIso8601();

    const FString Content = FString::Printf(
        TEXT("%s\n%s\n%s\n%s\n%.*f"),
        *Item.Key,
        *Item.Content,
        *SerializeEmbedding(Item.Embedding),
        *IsoTs,
        9, // precision similar to default float ToString
        static_cast<double>(Item.RelevanceScore)
    );

    return FFileHelper::SaveStringToFile(Content, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
}

bool FDiskBasedLongTermMemory::RetrieveItem(const FString& Key, FMemoryItem& OutItem) const
{
    const FString FilePath = FPaths::Combine(StoragePath, Key + TEXT(".txt"));
    if (!FPaths::FileExists(FilePath))
    {
        return false;
    }

    TArray<FString> Lines;
    if (!FFileHelper::LoadFileToStringArray(Lines, *FilePath))
    {
        return false;
    }

    if (Lines.Num() < 5)
    {
        return false;
    }

    const FString& KeyLine = Lines[0];
    const FString& ContentLine = Lines[1];
    const FString& EmbeddingLine = Lines[2];
    const FString& TimestampLine = Lines[3];
    const FString& RelevanceLine = Lines[4];

    TArray<float> Embedding;
    if (!ParseEmbedding(EmbeddingLine, Embedding))
    {
        return false;
    }

    FDateTime Timestamp;
    if (!FDateTime::ParseIso8601(*TimestampLine, Timestamp))
    {
        // Try parse as default if iso fails
        if (!FDateTime::Parse(TimestampLine, Timestamp))
        {
            return false;
        }
    }

    float Relevance = 0.f;
    if (!LexTryParseString(Relevance, *RelevanceLine))
    {
        return false;
    }

    OutItem = FMemoryItem(KeyLine, ContentLine, Embedding);
    OutItem.Timestamp = Timestamp;
    OutItem.RelevanceScore = Relevance;
    return true;
}
