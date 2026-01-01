#include "IsoMountManagerLibrary.h"

#include "HAL/PlatformProcess.h"

static void RunPowerShellCommand(const FString& CommandBody, bool& bSuccess, FString& OutStdOut, FString& OutError)
{
    bSuccess = false;
    OutStdOut.Reset();
    OutError.Reset();

    const FString Executable = TEXT("powershell.exe");
    const FString Args = FString::Printf(TEXT("-NoProfile -NonInteractive -Command \"%s\""), *CommandBody);

    void* ReadPipe = nullptr;
    void* WritePipe = nullptr;
    if (!FPlatformProcess::CreatePipe(ReadPipe, WritePipe))
    {
        OutError = TEXT("Failed to create process pipes.");
        return;
    }

    FProcHandle Proc = FPlatformProcess::CreateProc(*Executable, *Args, /*bLaunchDetached*/false, /*bLaunchHidden*/true, /*bLaunchReallyHidden*/true, nullptr, 0, nullptr, WritePipe);
    if (!Proc.IsValid())
    {
        FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
        OutError = TEXT("Failed to start powershell.exe. Ensure PowerShell is available.");
        return;
    }

    while (FPlatformProcess::IsProcRunning(Proc))
    {
        const FString Chunk = FPlatformProcess::ReadPipe(ReadPipe);
        if (!Chunk.IsEmpty())
        {
            OutStdOut += Chunk;
        }
        FPlatformProcess::Sleep(0.05f);
    }

    // Drain remaining output
    {
        const FString Tail = FPlatformProcess::ReadPipe(ReadPipe);
        if (!Tail.IsEmpty())
        {
            OutStdOut += Tail;
        }
    }

    int32 ReturnCode = -1;
    FPlatformProcess::GetProcReturnCode(Proc, &ReturnCode);

    FPlatformProcess::CloseProc(Proc);
    FPlatformProcess::ClosePipe(ReadPipe, WritePipe);

    if (ReturnCode == 0)
    {
        bSuccess = true;
    }
    else
    {
        OutError = FString::Printf(TEXT("PowerShell exited with code %d"), ReturnCode);
    }
}

void UIsoMountManagerLibrary::MountISO(const FString& IsoPath, bool& bSuccess, FString& OutStdOut, FString& OutError)
{
    const FString Cmd = FString::Printf(TEXT("Mount-DiskImage -ImagePath '%s'"), *IsoPath);
    RunPowerShellCommand(Cmd, bSuccess, OutStdOut, OutError);
}

void UIsoMountManagerLibrary::UnmountISO(const FString& IsoPath, bool& bSuccess, FString& OutStdOut, FString& OutError)
{
    const FString Cmd = FString::Printf(TEXT("Dismount-DiskImage -ImagePath '%s'"), *IsoPath);
    RunPowerShellCommand(Cmd, bSuccess, OutStdOut, OutError);
}

