#include "IsoAccessLibrary.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UIsoAccessLibrary::ReadFileFromISO(const FString& RelativePath, FString& OutContent, bool& bSuccess, FString& OutError)
{
    bSuccess = false;
    OutError.Reset();
    OutContent.Reset();

    // Build absolute path on Z: drive
    FString IsoPath = TEXT("Z:\\");
    if (!RelativePath.IsEmpty())
    {
        if (RelativePath.StartsWith(TEXT("/")) || RelativePath.StartsWith(TEXT("\\")))
        {
            IsoPath += RelativePath.Mid(1);
        }
        else
        {
            IsoPath += RelativePath;
        }
    }

    if (!FPaths::FileExists(IsoPath))
    {
        UE_LOG(LogTemp, Log, TEXT("File %s not found on the ISO."), *RelativePath);
        OutError = FString::Printf(TEXT("File not found: %s"), *IsoPath);
        return;
    }

    if (!FFileHelper::LoadFileToString(OutContent, *IsoPath))
    {
        OutError = FString::Printf(TEXT("Failed to read file: %s"), *IsoPath);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("File Content: %s"), *OutContent);
    bSuccess = true;
}

