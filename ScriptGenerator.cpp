#include "ScriptGenerator.h"

#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(LogScriptGenerator);

bool FScriptGenerator::GenerateScript(const FString& ClassName, const FString& Content)
{
    const FString ScriptsDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Scripts"));
    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    if (!PF.CreateDirectoryTree(*ScriptsDir))
    {
        UE_LOG(LogScriptGenerator, Error, TEXT("Failed to create Scripts directory: %s"), *ScriptsDir);
        return false;
    }

    const FString Path = FPaths::Combine(ScriptsDir, ClassName + TEXT(".cs"));
    if (!FFileHelper::SaveStringToFile(Content, *Path))
    {
        UE_LOG(LogScriptGenerator, Error, TEXT("Failed to write file: %s"), *Path);
        return false;
    }

    UE_LOG(LogScriptGenerator, Log, TEXT("Script %s.cs generated at %s"), *ClassName, *Path);
    return true;
}

