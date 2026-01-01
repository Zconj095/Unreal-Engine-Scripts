#include "LayeredShortTermMemory.h"

void ULayeredShortTermMemory::Initialize(int32 L1Size, int32 L2Size, int32 L3Size, int64 RamSizeBytes)
{
    L1Capacity = FMath::Max(0, L1Size);
    L2Capacity = FMath::Max(0, L2Size);
    L3Capacity = FMath::Max(0, L3Size);
    RamCapacityBytes = FMath::Max<int64>(0, RamSizeBytes);

    L1Cache.Reset();
    L2Cache.Reset();
    L3Cache.Reset();
    RamMemory.Reset();
}

void ULayeredShortTermMemory::AddItem(const FMemoryItem& Item)
{
    if (L1Cache.Num() < L1Capacity)
    {
        L1Cache.Add(Item);
        return;
    }
    if (L2Cache.Num() < L2Capacity)
    {
        L2Cache.Add(Item);
        return;
    }
    if (L3Cache.Num() < L3Capacity)
    {
        L3Cache.Add(Item);
        return;
    }

    const int64 ItemBytes = static_cast<int64>(Item.Embedding.Num()) * static_cast<int64>(sizeof(float));
    if (GetRamUsageInternal() + ItemBytes <= RamCapacityBytes)
    {
        RamMemory.Add(Item);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RAM memory limit reached. Item '%s' cannot be stored."), *Item.Key);
    }
}

bool ULayeredShortTermMemory::RetrieveItem(const FString& Key, FMemoryItem& OutItem) const
{
    for (const FMemoryItem& It : L1Cache)
    {
        if (It.Key == Key) { OutItem = It; return true; }
    }
    for (const FMemoryItem& It : L2Cache)
    {
        if (It.Key == Key) { OutItem = It; return true; }
    }
    for (const FMemoryItem& It : L3Cache)
    {
        if (It.Key == Key) { OutItem = It; return true; }
    }
    if (const FMemoryItem* Found = RamMemory.FindByPredicate([&Key](const FMemoryItem& I){ return I.Key == Key; }))
    {
        OutItem = *Found; return true;
    }
    return false;
}

int64 ULayeredShortTermMemory::GetCurrentRamUsage() const
{
    return GetRamUsageInternal();
}

int64 ULayeredShortTermMemory::GetRamUsageInternal() const
{
    int64 Total = 0;
    for (const FMemoryItem& It : RamMemory)
    {
        Total += static_cast<int64>(It.Embedding.Num()) * static_cast<int64>(sizeof(float));
    }
    return Total;
}

