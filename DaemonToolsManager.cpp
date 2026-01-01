#include "DaemonToolsManager.h"

#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY(LogDaemonTools);

namespace
{
#if PLATFORM_WINDOWS
    static const TCHAR* GDefaultDaemonToolsPath = TEXT("C:\\Program Files\\DAEMON Tools Lite\\dtlite.exe");
#endif
}

FString FDaemonToolsManager::GetDaemonToolsPath()
{
#if PLATFORM_WINDOWS
    return FString(GDefaultDaemonToolsPath);
#else
    return FString();
#endif
}

bool FDaemonToolsManager::MountISO(const FString& IsoPath, int32 DriveIndex /*= 0*/)
{
#if PLATFORM_WINDOWS
    const FString DaemonToolsPath = GetDaemonToolsPath();
    if (!FPaths::FileExists(DaemonToolsPath))
    {
        UE_LOG(LogDaemonTools, Error, TEXT("DAEMON Tools not found at path: %s"), *DaemonToolsPath);
        return false;
    }

    // Example: -mount dt, 0, "C:\\path\\to\\image.iso"
    const FString Args = FString::Printf(TEXT("-mount dt, %d, \"%s\""), DriveIndex, *IsoPath);

    int32 ReturnCode = -1;
    FString StdOut, StdErr;
    const bool bStarted = FPlatformProcess::ExecProcess(*DaemonToolsPath, *Args, &ReturnCode, &StdOut, &StdErr);

    UE_LOG(LogDaemonTools, Log, TEXT("MountISO stdout:\n%s"), *StdOut);
    if (!StdErr.IsEmpty())
    {
        UE_LOG(LogDaemonTools, Warning, TEXT("MountISO stderr:\n%s"), *StdErr);
    }

    if (!bStarted)
    {
        UE_LOG(LogDaemonTools, Error, TEXT("Failed to start DAEMON Tools process."));
        return false;
    }

    if (ReturnCode != 0)
    {
        UE_LOG(LogDaemonTools, Error, TEXT("DAEMON Tools returned non-zero code: %d"), ReturnCode);
        return false;
    }

    return true;
#else
    UE_LOG(LogDaemonTools, Error, TEXT("MountISO is only supported on Windows."));
    return false;
#endif
}

bool FDaemonToolsManager::UnmountISO(int32 DriveIndex /*= 0*/)
{
#if PLATFORM_WINDOWS
    const FString DaemonToolsPath = GetDaemonToolsPath();
    if (!FPaths::FileExists(DaemonToolsPath))
    {
        UE_LOG(LogDaemonTools, Error, TEXT("DAEMON Tools not found at path: %s"), *DaemonToolsPath);
        return false;
    }

    // Example: -unmount 0
    const FString Args = FString::Printf(TEXT("-unmount %d"), DriveIndex);

    int32 ReturnCode = -1;
    FString StdOut, StdErr;
    const bool bStarted = FPlatformProcess::ExecProcess(*DaemonToolsPath, *Args, &ReturnCode, &StdOut, &StdErr);

    UE_LOG(LogDaemonTools, Log, TEXT("UnmountISO stdout:\n%s"), *StdOut);
    if (!StdErr.IsEmpty())
    {
        UE_LOG(LogDaemonTools, Warning, TEXT("UnmountISO stderr:\n%s"), *StdErr);
    }

    if (!bStarted)
    {
        UE_LOG(LogDaemonTools, Error, TEXT("Failed to start DAEMON Tools process."));
        return false;
    }

    if (ReturnCode != 0)
    {
        UE_LOG(LogDaemonTools, Error, TEXT("DAEMON Tools returned non-zero code: %d"), ReturnCode);
        return false;
    }

    return true;
#else
    UE_LOG(LogDaemonTools, Error, TEXT("UnmountISO is only supported on Windows."));
    return false;
#endif
}

