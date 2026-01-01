#include "DllImporter.h"

#include "HAL/PlatformProcess.h"

DEFINE_LOG_CATEGORY(LogDllImporter);

void* FDllImporter::LoadLibrary(const FString& DllPath)
{
    void* Handle = FPlatformProcess::GetDllHandle(*DllPath);
    if (!Handle)
    {
        UE_LOG(LogDllImporter, Error, TEXT("GetDllHandle failed for: %s"), *DllPath);
    }
    return Handle;
}

void* FDllImporter::GetProcAddress(void* ModuleHandle, const TCHAR* ProcedureName)
{
    if (!ModuleHandle)
    {
        UE_LOG(LogDllImporter, Error, TEXT("GetProcAddress called with null module handle for function '%s'"), ProcedureName);
        return nullptr;
    }
    void* Addr = FPlatformProcess::GetDllExport(ModuleHandle, ProcedureName);
    if (!Addr)
    {
        UE_LOG(LogDllImporter, Error, TEXT("GetDllExport failed for function '%s'"), ProcedureName);
    }
    return Addr;
}

void FDllImporter::FreeLibrary(void* ModuleHandle)
{
    if (ModuleHandle)
    {
        FPlatformProcess::FreeDllHandle(ModuleHandle);
    }
}

