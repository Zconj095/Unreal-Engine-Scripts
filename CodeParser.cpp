#include "CodeParser.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

FString UCodeParser::Parse_Implementation(const FString& FilePath)
{
    LastFilePath = FilePath;
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        // Return empty string on failure to mirror simple behavior
        Content.Reset();
    }
    return Content;
}

FString UCodeParser::GetFileType_Implementation() const
{
    if (LastFilePath.IsEmpty())
    {
        return TEXT("Unknown File Type");
    }

    const FString Ext = FPaths::GetExtension(LastFilePath, /*bIncludeDot*/ false).ToLower();
    if (Ext == TEXT("cs"))
    {
        return TEXT("C# Code");
    }
    if (Ext == TEXT("py"))
    {
        return TEXT("Python Code");
    }
    if (Ext == TEXT("java"))
    {
        return TEXT("Java Code");
    }
    if (Ext == TEXT("cpp") || Ext == TEXT("cc") || Ext == TEXT("cxx") || Ext == TEXT("c++"))
    {
        return TEXT("C++ Code");
    }
    return TEXT("Unknown Code");
}

