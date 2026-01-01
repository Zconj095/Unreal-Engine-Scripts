#include "ProjectHardwareInfo.h"

#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY(LogProjectHardwareInfo);

namespace
{
    static bool ExecProcessCapture(const FString& Exe, const FString& Args, int32& OutCode, FString& OutStdOut, FString& OutStdErr)
    {
        return FPlatformProcess::ExecProcess(*Exe, *Args, &OutCode, &OutStdOut, &OutStdErr);
    }
}

int64 FProjectHardwareInfo::GetL1CacheSize()
{
#if PLATFORM_WINDOWS
    return GetWindowsCacheSize(1);
#elif PLATFORM_LINUX
    return GetLinuxCacheSize(1);
#elif PLATFORM_MAC
    return GetMacCacheSize(TEXT("hw.l1icachesize"));
#else
    UE_LOG(LogProjectHardwareInfo, Warning, TEXT("GetL1CacheSize not supported on this platform."));
    return 0;
#endif
}

int64 FProjectHardwareInfo::GetL2CacheSize()
{
#if PLATFORM_WINDOWS
    return GetWindowsCacheSize(2);
#elif PLATFORM_LINUX
    return GetLinuxCacheSize(2);
#elif PLATFORM_MAC
    return GetMacCacheSize(TEXT("hw.l2cachesize"));
#else
    UE_LOG(LogProjectHardwareInfo, Warning, TEXT("GetL2CacheSize not supported on this platform."));
    return 0;
#endif
}

int64 FProjectHardwareInfo::GetL3CacheSize()
{
#if PLATFORM_WINDOWS
    return GetWindowsCacheSize(3);
#elif PLATFORM_LINUX
    return GetLinuxCacheSize(3);
#elif PLATFORM_MAC
    return GetMacCacheSize(TEXT("hw.l3cachesize"));
#else
    UE_LOG(LogProjectHardwareInfo, Warning, TEXT("GetL3CacheSize not supported on this platform."));
    return 0;
#endif
}

int64 FProjectHardwareInfo::GetWindowsCacheSize(int32 CacheLevel)
{
#if PLATFORM_WINDOWS
    // Use PowerShell CIM query for cache memory. InstalledSize is in KB.
    const FString Exe = TEXT("powershell");
    const FString PS = TEXT("-NoProfile -NonInteractive -Command \"")
        TEXT("$items = Get-CimInstance Win32_CacheMemory; ")
        TEXT("$items | Select-Object Level,InstalledSize | ConvertTo-Csv -NoTypeInformation\"");

    int32 Code = -1; FString Out, Err;
    if (!ExecProcessCapture(Exe, PS, Code, Out, Err) || Code != 0)
    {
        UE_LOG(LogProjectHardwareInfo, Warning, TEXT("PowerShell cache query failed: %s"), *Err);
        return 0;
    }

    TArray<FString> Lines;
    Out.ParseIntoArrayLines(Lines, /*CullEmpty*/true);
    if (Lines.Num() <= 1)
    {
        return 0; // no rows
    }

    // CSV header: "Level","InstalledSize"
    int64 TotalBytes = 0;
    for (int32 i = 1; i < Lines.Num(); ++i)
    {
        const FString& L = Lines[i];
        TArray<FString> Fields;
        L.ParseIntoArray(Fields, TEXT(","), /*CullEmpty*/false);
        if (Fields.Num() < 2) continue;

        auto TrimQuotes = [](FString S){ S.TrimStartAndEndInline(); if (S.StartsWith(TEXT("\"")) && S.EndsWith(TEXT("\"")) && S.Len() >= 2) { S = S.Mid(1, S.Len()-2); } return S; };
        const FString LevelStr = TrimQuotes(Fields[0]);
        const FString SizeKbStr = TrimQuotes(Fields[1]);

        int32 LevelVal = 0; int32 SizeKB = 0;
        if (LexTryParseString(LevelVal, *LevelStr) && LexTryParseString(SizeKB, *SizeKbStr))
        {
            if (LevelVal == CacheLevel)
            {
                TotalBytes += static_cast<int64>(SizeKB) * 1024ll;
            }
        }
    }

    return TotalBytes;
#else
    return 0;
#endif
}

int64 FProjectHardwareInfo::GetLinuxCacheSize(int32 CacheLevel)
{
#if PLATFORM_LINUX
    // Read /sys/devices/system/cpu/cpu0/cache/index{level-1}/size
    const int32 Index = CacheLevel - 1;
    const FString Path = FString::Printf(TEXT("/sys/devices/system/cpu/cpu0/cache/index%d/size"), Index);
    FString SizeStr;
    if (!FFileHelper::LoadFileToString(SizeStr, *Path))
    {
        return 0;
    }
    SizeStr.TrimStartAndEndInline();
    if (SizeStr.EndsWith(TEXT("K")))
    {
        SizeStr.RemoveAt(SizeStr.Len()-1, 1, EAllowShrinking::No);
        int64 K = 0; if (LexTryParseString(K, *SizeStr)) return K * 1024ll;
    }
    else if (SizeStr.EndsWith(TEXT("M")))
    {
        SizeStr.RemoveAt(SizeStr.Len()-1, 1, EAllowShrinking::No);
        int64 M = 0; if (LexTryParseString(M, *SizeStr)) return M * 1024ll * 1024ll;
    }
    else
    {
        int64 B = 0; if (LexTryParseString(B, *SizeStr)) return B; // assume bytes
    }
    return 0;
#else
    return 0;
#endif
}

int64 FProjectHardwareInfo::GetMacCacheSize(const TCHAR* SysctlKey)
{
#if PLATFORM_MAC
    const FString Exe = TEXT("/usr/sbin/sysctl");
    const FString Args = FString::Printf(TEXT("%s"), SysctlKey);
    int32 Code = -1; FString Out, Err;
    if (!ExecProcessCapture(Exe, Args, Code, Out, Err) || Code != 0)
    {
        return 0;
    }
    // Expected: key: value
    int32 SepIdx = INDEX_NONE;
    if (Out.FindChar(TEXT(':'), SepIdx))
    {
        const FString ValStr = Out.Mid(SepIdx + 1).TrimStartAndEnd();
        int64 Bytes = 0; if (LexTryParseString(Bytes, *ValStr)) return Bytes;
    }
    return 0;
#else
    return 0;
#endif
}
