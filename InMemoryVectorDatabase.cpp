#include "InMemoryVectorDatabase.h"

void UInMemoryVectorDatabase::AddVector(int32 VectorId, const TArray<float>& Vector)
{
    FFloatArray Wrap; Wrap.Values = Vector;
    Storage.Add(VectorId, MoveTemp(Wrap));
}

void UInMemoryVectorDatabase::GetVector_Implementation(int32 VectorId, TArray<float>& OutVector) const
{
    if (const FFloatArray* Found = Storage.Find(VectorId))
    {
        OutVector = Found->Values;
    }
    else
    {
        OutVector.Reset();
    }
}

bool UInMemoryVectorDatabase::RemoveVector(int32 VectorId)
{
    return Storage.Remove(VectorId) > 0;
}

void UInMemoryVectorDatabase::GetAllEntries(TArray<int32>& OutIds, TArray<FFloatArray>& OutVectors) const
{
    OutIds.Reset();
    OutVectors.Reset();
    OutIds.Reserve(Storage.Num());
    OutVectors.Reserve(Storage.Num());
    for (const TPair<int32, FFloatArray>& P : Storage)
    {
        OutIds.Add(P.Key);
        OutVectors.Add(P.Value);
    }
}

