#include "MemoryModule.h"

void UMemoryModule::Remember(const FString& Key, const FString& Value)
{
    // Maintain short-term memory capacity (remove oldest first)
    TrimShortTerm();
    ShortTermMemory.Add(Value);

    // Store in long-term only if key is new
    if (!Key.IsEmpty() && !LongTermMemory.Contains(Key))
    {
        LongTermMemory.Add(Key, Value);
    }
}

FString UMemoryModule::Recall(const FString& Key) const
{
    if (const FString* Found = LongTermMemory.Find(Key))
    {
        return *Found;
    }
    return TEXT("No memory found.");
}

void UMemoryModule::TrimShortTerm()
{
    while (ShortTermMemory.Num() >= ShortTermCapacity && ShortTermMemory.Num() > 0)
    {
        ShortTermMemory.RemoveAt(0);
    }
}

