#include "FileParserManager.h"

#include "DocxParser.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformProcess.h"
#include "XmlFile.h"

// Local helper parsers implementing IFileParser
namespace
{
    class FTxtParser final : public IFileParser
    {
    public:
        virtual FString Parse(const FString& FilePath) override
        {
            FString Out;
            if (!FFileHelper::LoadFileToString(Out, *FilePath))
            {
                return FString::Printf(TEXT("Error: failed to read text file: %s"), *FilePath);
            }
            return Out;
        }

        virtual FString GetFileType() const override { return TEXT("TXT"); }
    };

    class FCodeParser final : public IFileParser
    {
    public:
        virtual FString Parse(const FString& FilePath) override
        {
            FString Out;
            if (!FFileHelper::LoadFileToString(Out, *FilePath))
            {
                return FString::Printf(TEXT("Error: failed to read code file: %s"), *FilePath);
            }
            return Out;
        }

        virtual FString GetFileType() const override { return TEXT("Code"); }
    };

    class FRtfParser final : public IFileParser
    {
    public:
        virtual FString Parse(const FString& FilePath) override
        {
            FString Out;
            if (!FFileHelper::LoadFileToString(Out, *FilePath))
            {
                return FString::Printf(TEXT("Error: failed to read RTF file: %s"), *FilePath);
            }
            return Out; // raw content; no RTF decoding here
        }

        virtual FString GetFileType() const override { return TEXT("RTF"); }
    };

    class FOdtParser final : public IFileParser
    {
    public:
        virtual FString Parse(const FString& FilePath) override
        {
            if (!FPaths::FileExists(FilePath))
            {
                return FString::Printf(TEXT("Error while parsing ODT: File not found: %s"), *FilePath);
            }

#if PLATFORM_WINDOWS
            // Create temp extract dir
            const FString TempRoot = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("OdtTemp"));
            const FString ExtractDir = FPaths::Combine(TempRoot, FGuid::NewGuid().ToString(EGuidFormats::Digits));
            IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
            PF.CreateDirectoryTree(*ExtractDir);

            // Expand-Archive via PowerShell
            const FString PSExe = TEXT("powershell");
            const FString PSArgs = FString::Printf(TEXT("-NoProfile -NonInteractive -Command \"Expand-Archive -LiteralPath '%s' -DestinationPath '%s' -Force\""), *FilePath, *ExtractDir);
            FString StdOut, StdErr; int32 Code = -1;
            const bool bOk = FPlatformProcess::ExecProcess(*PSExe, *PSArgs, &Code, &StdOut, &StdErr);
            if (!bOk || Code != 0)
            {
                IFileManager::Get().DeleteDirectory(*ExtractDir, /*RequireExists*/false, /*Tree*/true);
                return FString::Printf(TEXT("Error while parsing ODT: Failed to extract archive. %s%s%s"),
                    StdErr.IsEmpty() ? TEXT("") : TEXT("stderr: "),
                    StdErr.IsEmpty() ? TEXT("") : *StdErr,
                    StdOut.IsEmpty() ? TEXT("") : *FString::Printf(TEXT(" stdout: %s"), *StdOut));
            }

            // content.xml at root of ODT
            const FString ContentXml = FPaths::Combine(ExtractDir, TEXT("content.xml"));
            if (!FPaths::FileExists(ContentXml))
            {
                IFileManager::Get().DeleteDirectory(*ExtractDir, /*RequireExists*/false, /*Tree*/true);
                return TEXT("Error while parsing ODT: The ODT file does not contain a content.xml file.");
            }

            FString OutText;
            {
                FXmlFile XmlFile;
                if (!XmlFile.LoadFile(ContentXml, EConstructMethod::ConstructFromFile))
                {
                    IFileManager::Get().DeleteDirectory(*ExtractDir, /*RequireExists*/false, /*Tree*/true);
                    return FString::Printf(TEXT("Error while parsing ODT: Failed to load XML: %s"), *ContentXml);
                }

                // Recursively collect text from <text:p> elements (or any tag ending with :p)
                struct FLocal
                {
                    static void Collect(const FXmlNode* Node, FString& Out)
                    {
                        if (!Node) return;
                        const FString Tag = Node->GetTag();
                        if (Tag == TEXT("text:p") || Tag.EndsWith(TEXT(":p")))
                        {
                            const FString C = Node->GetContent();
                            if (!C.IsEmpty())
                            {
                                Out.Append(C);
                                Out.AppendChar('\n');
                            }
                        }
                        const TArray<FXmlNode*>& Children = Node->GetChildrenNodes();
                        for (const FXmlNode* Child : Children) { Collect(Child, Out); }
                    }
                };

                FLocal::Collect(XmlFile.GetRootNode(), OutText);
            }

            IFileManager::Get().DeleteDirectory(*ExtractDir, /*RequireExists*/false, /*Tree*/true);
            return OutText;
#else
            return TEXT("Error while parsing ODT: Extraction requires Windows PowerShell on this platform.");
#endif
        }

        virtual FString GetFileType() const override { return TEXT("ODT"); }
    };

    class FPdfParser final : public IFileParser
    {
    public:
        virtual FString Parse(const FString& FilePath) override
        {
            return FString::Printf(TEXT("Error while parsing PDF: Not implemented for %s"), *FilePath);
        }

        virtual FString GetFileType() const override { return TEXT("PDF"); }
    };

    class FDocxParserAdapter final : public IFileParser
    {
    public:
        virtual FString Parse(const FString& FilePath) override
        {
            return FDocxParser::Parse(FilePath);
        }

        virtual FString GetFileType() const override { return FDocxParser::GetFileType(); }
    };
}

FFileParserManager::FFileParserManager()
{
    // Register default parsers
    RegisterParser(TEXT(".pdf"), MakeShared<FPdfParser>());
    RegisterParser(TEXT(".txt"), MakeShared<FTxtParser>());
    RegisterParser(TEXT(".docx"), MakeShared<FDocxParserAdapter>());
    RegisterParser(TEXT(".odt"), MakeShared<FOdtParser>());
    RegisterParser(TEXT(".rtf"), MakeShared<FRtfParser>());
    RegisterParser(TEXT(".cs"), MakeShared<FCodeParser>());
    RegisterParser(TEXT(".py"), MakeShared<FCodeParser>());
}

void FFileParserManager::RegisterParser(const FString& ExtensionWithDot, TSharedRef<IFileParser> Parser)
{
    FString Key = ExtensionWithDot;
    Key.ToLowerInline();
    Parsers.Add(Key, Parser);
}

TSharedPtr<IFileParser> FFileParserManager::FindParserForExtension(const FString& ExtensionWithDotLower) const
{
    if (const TSharedPtr<IFileParser>* Found = Parsers.Find(ExtensionWithDotLower))
    {
        return *Found;
    }
    return nullptr;
}

FString FFileParserManager::ParseFile(const FString& FilePath) const
{
    const FString Ext = FPaths::GetExtension(FilePath, /*bIncludeDot*/true).ToLower();
    TSharedPtr<IFileParser> Parser = FindParserForExtension(Ext);
    if (Parser.IsValid())
    {
        return Parser->Parse(FilePath);
    }
    return FString::Printf(TEXT("File format %s is not supported."), *Ext);
}

FString FFileParserManager::GetFileType(const FString& FilePath) const
{
    const FString Ext = FPaths::GetExtension(FilePath, /*bIncludeDot*/true).ToLower();
    TSharedPtr<IFileParser> Parser = FindParserForExtension(Ext);
    if (Parser.IsValid())
    {
        return Parser->GetFileType();
    }
    return TEXT("Unknown");
}
