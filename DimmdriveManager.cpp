#include "DimmdriveManager.h"

#include "HAL/PlatformProcess.h"

DEFINE_LOG_CATEGORY(LogDimmdrive);

bool FDimmdriveManager::RunDimmdrive(const FString& Arguments, const TCHAR* ContextLabel)
{
#if PLATFORM_WINDOWS
    const FString Exe = TEXT("dimmdrive.exe"); // assumes in PATH; adjust if needed

    int32 ReturnCode = -1;
    FString StdOut, StdErr;
    const bool bStarted = FPlatformProcess::ExecProcess(*Exe, *Arguments, &ReturnCode, &StdOut, &StdErr);

    UE_LOG(LogDimmdrive, Log, TEXT("%s stdout:\n%s"), ContextLabel, *StdOut);
    if (!StdErr.IsEmpty())
    {
        UE_LOG(LogDimmdrive, Warning, TEXT("%s stderr:\n%s"), ContextLabel, *StdErr);
    }

    if (!bStarted)
    {
        UE_LOG(LogDimmdrive, Error, TEXT("%s: failed to start dimmdrive.exe. Is it installed and on PATH?"), ContextLabel);
        return false;
    }

    if (ReturnCode != 0)
    {
        UE_LOG(LogDimmdrive, Error, TEXT("%s: dimmdrive.exe returned non-zero code: %d"), ContextLabel, ReturnCode);
        return false;
    }

    return true;
#else
    UE_LOG(LogDimmdrive, Error, TEXT("Dimmdrive is only supported on Windows."));
    return false;
#endif
}

bool FDimmdriveManager::CreateRamDisk(const FString& DriveLetter, int32 SizeMB)
{
    const FString Args = FString::Printf(TEXT("-createDisk driveLetter=%s size=%dMB"), *DriveLetter, SizeMB);
    return RunDimmdrive(Args, TEXT("CreateRamDisk"));
}

bool FDimmdriveManager::DeleteRamDisk(const FString& DriveLetter)
{
    const FString Args = FString::Printf(TEXT("-deleteDisk driveLetter=%s"), *DriveLetter);
    return RunDimmdrive(Args, TEXT("DeleteRamDisk"));
}

