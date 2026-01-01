#include "DocxParser.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformProcess.h"
#include "XmlFile.h"

namespace
{
    static FString MakeDefaultImageOutputFolder()
    {
        return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("ExtractedImages"));
    }

    static FString CreateTempExtractDir()
    {
        const FString TempRoot = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("DocxTemp"));
        const FString TempDir = FPaths::Combine(TempRoot, FGuid::NewGuid().ToString(EGuidFormats::Digits));
        IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
        PF.CreateDirectoryTree(*TempDir);
        return TempDir;
    }

    static void CleanupDir(const FString& Dir)
    {
        IFileManager::Get().DeleteDirectory(*Dir, /*RequireExists*/false, /*Tree*/true);
    }

    static bool ExtractDocxWithPowerShell(const FString& DocxPath, const FString& DestDir, FString& OutStdOut, FString& OutStdErr, int32& OutCode)
    {
#if PLATFORM_WINDOWS
        const FString PSExe = TEXT("powershell");
        const FString PSArgs = FString::Printf(TEXT("-NoProfile -NonInteractive -Command \"Expand-Archive -LiteralPath '%s' -DestinationPath '%s' -Force\""), *DocxPath, *DestDir);
        return FPlatformProcess::ExecProcess(*PSExe, *PSArgs, &OutCode, &OutStdOut, &OutStdErr);
#else
        OutStdErr = TEXT("Docx parsing requires Windows PowerShell for extraction on this platform.");
        OutStdOut.Reset();
        OutCode = -1;
        return false;
#endif
    }

    static void CollectAllWTText(const FXmlNode* Node, FString& OutText)
    {
        if (!Node) return;

        const FString Tag = Node->GetTag();
        if (Tag == TEXT("w:t") || Tag.EndsWith(TEXT(":t")))
        {
            const FString Content = Node->GetContent();
            if (!Content.IsEmpty())
            {
                OutText.Append(Content);
                OutText.AppendChar('\n');
            }
        }

        const TArray<FXmlNode*>& Children = Node->GetChildrenNodes();
        for (const FXmlNode* Child : Children)
        {
            CollectAllWTText(Child, OutText);
        }
    }
}

FString FDocxParser::Parse(const FString& FilePath, const FString& ImageOutputFolder)
{
    if (!FPaths::FileExists(FilePath))
    {
        return FString::Printf(TEXT("Error while parsing DOCX: File not found: %s"), *FilePath);
    }

    const FString ExtractDir = CreateTempExtractDir();

    FString StdOut, StdErr;
    int32 ReturnCode = -1;
    const bool bOk = ExtractDocxWithPowerShell(FilePath, ExtractDir, StdOut, StdErr, ReturnCode);
    if (!bOk || ReturnCode != 0)
    {
        CleanupDir(ExtractDir);
        const FString ErrMsg = FString::Printf(TEXT("Error while parsing DOCX: Failed to extract archive. %s%s%s"),
            StdErr.IsEmpty() ? TEXT("") : TEXT("stderr: "),
            StdErr.IsEmpty() ? TEXT("") : *StdErr,
            StdOut.IsEmpty() ? TEXT("") : *FString::Printf(TEXT(" stdout: %s"), *StdOut));
        return ErrMsg;
    }

    const FString DocumentXml = FPaths::Combine(ExtractDir, TEXT("word"), TEXT("document.xml"));
    if (!FPaths::FileExists(DocumentXml))
    {
        CleanupDir(ExtractDir);
        return TEXT("Error while parsing DOCX: The DOCX file does not contain a document.xml file.");
    }

    FString TextContent;
    {
        FXmlFile XmlFile;
        if (!XmlFile.LoadFile(DocumentXml, EConstructMethod::ConstructFromFile))
        {
            CleanupDir(ExtractDir);
            return FString::Printf(TEXT("Error while parsing DOCX: Failed to load XML: %s"), *DocumentXml);
        }

        const FXmlNode* Root = XmlFile.GetRootNode();
        CollectAllWTText(Root, TextContent);
    }

    // Prepare image output folder
    FString FinalImageOut = ImageOutputFolder.IsEmpty() ? MakeDefaultImageOutputFolder() : ImageOutputFolder;
    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    PF.CreateDirectoryTree(*FinalImageOut);

    // Copy images
    TArray<FString> CopiedImagePaths;
    {
        const FString MediaDir = FPaths::Combine(ExtractDir, TEXT("word"), TEXT("media"));
        if (PF.DirectoryExists(*MediaDir))
        {
            TArray<FString> Files;
            IFileManager::Get().FindFilesRecursive(Files, *MediaDir, TEXT("*.*"), /*Files*/true, /*Dirs*/false);
            for (const FString& Src : Files)
            {
                const FString Ext = FPaths::GetExtension(Src).ToLower();
                if (Ext == TEXT("jpeg") || Ext == TEXT("jpg") || Ext == TEXT("png") || Ext == TEXT("gif") || Ext == TEXT("bmp"))
                {
                    const FString Dest = FPaths::Combine(FinalImageOut, FPaths::GetCleanFilename(Src));
                    if (IFileManager::Get().Copy(*Dest, *Src) == COPY_OK)
                    {
                        CopiedImagePaths.Add(Dest);
                    }
                }
            }
        }
    }

    // Cleanup extracted temp
    CleanupDir(ExtractDir);

    // Build result string
    FString Result;
    Result.Append(TEXT("=== Text Content ===\n"));
    Result.Append(TextContent);
    Result.Append(TEXT("=== Images Extracted ===\n"));
    for (const FString& P : CopiedImagePaths)
    {
        Result.Append(P);
        Result.AppendChar('\n');
    }

    return Result;
}
