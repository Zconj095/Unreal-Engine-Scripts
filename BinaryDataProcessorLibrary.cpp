#include "BinaryDataProcessorLibrary.h"

#include "Misc/FileHelper.h"

void UBinaryDataProcessorLibrary::ProcessBinaryData(const FString& FilePath, TArray<float>& OutEmbedding, bool& bSuccess, FString& OutError)
{
    bSuccess = false;
    OutError.Reset();
    OutEmbedding.Reset();

    if (FilePath.IsEmpty())
    {
        OutError = TEXT("FilePath is empty");
        return;
    }

    TArray<uint8> Bytes;
    if (!FFileHelper::LoadFileToArray(Bytes, *FilePath))
    {
        OutError = FString::Printf(TEXT("Failed to read file: %s"), *FilePath);
        return;
    }

    OutEmbedding.SetNumUninitialized(Bytes.Num());
    for (int32 i = 0; i < Bytes.Num(); ++i)
    {
        OutEmbedding[i] = static_cast<float>(Bytes[i]) / 255.0f;
    }

    bSuccess = true;
}

