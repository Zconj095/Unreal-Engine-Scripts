#include "IsoGeneratorLibrary.h"

#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"

void UIsoGeneratorLibrary::GenerateISO(const FString& SourcePath, const FString& IsoPath, const FString& VolumeLabel, bool& bSuccess, FString& OutStdOut, FString& OutError)
{
    bSuccess = false;
    OutStdOut.Reset();
    OutError.Reset();

    if (SourcePath.IsEmpty() || IsoPath.IsEmpty())
    {
        OutError = TEXT("SourcePath or IsoPath is empty.");
        return;
    }

    // Build args: -o "IsoPath" -V "VolumeLabel" "SourcePath"
    const FString Args = FString::Printf(TEXT("-o \"%s\" -V \"%s\" \"%s\""), *IsoPath, *VolumeLabel, *SourcePath);

    void* ReadPipe = nullptr;
    void* WritePipe = nullptr;
    if (!FPlatformProcess::CreatePipe(ReadPipe, WritePipe))
    {
        OutError = TEXT("Failed to create process pipes.");
        return;
    }

    const FString Executable = TEXT("mkisofs.exe");
    FProcHandle Proc = FPlatformProcess::CreateProc(*Executable, *Args, /*bLaunchDetached*/false, /*bLaunchHidden*/true, /*bLaunchReallyHidden*/true, nullptr, 0, nullptr, WritePipe);
    if (!Proc.IsValid())
    {
        FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
        OutError = TEXT("Failed to start mkisofs.exe. Ensure it is installed and on PATH.");
        return;
    }

    // Read stdout while running
    while (FPlatformProcess::IsProcRunning(Proc))
    {
        const FString Chunk = FPlatformProcess::ReadPipe(ReadPipe);
        if (!Chunk.IsEmpty())
        {
            OutStdOut += Chunk;
        }
        FPlatformProcess::Sleep(0.05f);
    }

    // Drain any remaining output
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
        OutError = FString::Printf(TEXT("mkisofs exited with code %d"), ReturnCode);
    }
}

