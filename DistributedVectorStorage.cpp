#include "DistributedVectorStorage.h"

DEFINE_LOG_CATEGORY(LogDistributedVector);

FDistributedVectorStorage::FDistributedVectorStorage(const TArray<FString>& Nodes)
    : NodeAddresses(Nodes)
{
}

FString FDistributedVectorStorage::GetNodeForVector(int32 VectorId) const
{
    if (NodeAddresses.Num() == 0)
    {
        UE_LOG(LogDistributedVector, Warning, TEXT("No nodes configured; cannot select node for id %d"), VectorId);
        return FString();
    }
    const int32 Index = FMath::Abs(VectorId) % NodeAddresses.Num();
    return NodeAddresses[Index];
}

void FDistributedVectorStorage::StoreVector(int32 VectorId, const TArray<float>& Vector) const
{
    const FString Node = GetNodeForVector(VectorId);
    UE_LOG(LogDistributedVector, Log, TEXT("Storing vector %d on node %s (size=%d)"), VectorId, *Node, Vector.Num());
}

TArray<float> FDistributedVectorStorage::RetrieveVector(int32 VectorId) const
{
    const FString Node = GetNodeForVector(VectorId);
    UE_LOG(LogDistributedVector, Log, TEXT("Retrieving vector %d from node %s"), VectorId, *Node);
    TArray<float> Dummy;
    Dummy.SetNumZeroed(128);
    return Dummy;
}

