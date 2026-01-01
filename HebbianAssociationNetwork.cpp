#include "HebbianAssociationNetwork.h"

namespace
{
    static FORCEINLINE int64 PackKeyOrdered(int32 A, int32 B)
    {
        const int32 MinV = FMath::Min(A, B);
        const int32 MaxV = FMath::Max(A, B);
        return (static_cast<int64>(static_cast<uint32>(MinV)) << 32) | static_cast<uint64>(static_cast<uint32>(MaxV));
    }
}

int64 UHebbianAssociationNetwork::MakeKey(int32 A, int32 B)
{
    return PackKeyOrdered(A, B);
}

void UHebbianAssociationNetwork::StrengthenConnection(int32 MemoryId1, int32 MemoryId2, float Amount)
{
    if (MemoryId1 == MemoryId2)
    {
        return; // ignore self-edges
    }
    const int64 Key = MakeKey(MemoryId1, MemoryId2);
    float& W = Weights.FindOrAdd(Key);
    W += Amount;
}

TArray<int32> UHebbianAssociationNetwork::GetAssociatedMemoriesBP(int32 MemoryId) const
{
    return GetAssociatedMemories(MemoryId);
}

TArray<int32> UHebbianAssociationNetwork::GetAssociatedMemories(int32 NodeId) const
{
    struct FPair { int32 OtherId; float Weight; };
    TArray<FPair> Assoc; Assoc.Reserve(Weights.Num());

    for (const TPair<int64, float>& It : Weights)
    {
        const int64 Key = It.Key;
        const float W = It.Value;
        const int32 A = static_cast<int32>(static_cast<uint32>(Key >> 32));
        const int32 B = static_cast<int32>(static_cast<uint32>(Key & 0xFFFFFFFFull));
        if (A == NodeId)
        {
            Assoc.Add({ B, W });
        }
        else if (B == NodeId)
        {
            Assoc.Add({ A, W });
        }
    }

    Assoc.Sort([](const FPair& X, const FPair& Y){ return X.Weight > Y.Weight; });

    TArray<int32> Result; Result.Reserve(Assoc.Num());
    for (const FPair& P : Assoc)
    {
        Result.Add(P.OtherId);
    }
    return Result;
}

