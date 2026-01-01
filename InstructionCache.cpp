#include "InstructionCache.h"

void UInstructionCache::AddToCache(const FString& Instruction, UObject* Result)
{
    Cache.Add(Instruction, Result);
}

const UObject* UInstructionCache::GetFromCache(const FString& Instruction) const
{
    if (const UObject* const* Found = Cache.Find(Instruction))
    {
        return *Found;
    }
    return nullptr;
}

bool UInstructionCache::IsInCache(const FString& Instruction) const
{
    return Cache.Contains(Instruction);
}
