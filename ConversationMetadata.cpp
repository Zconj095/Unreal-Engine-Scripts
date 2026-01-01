#include "ConversationMetadata.h"

void UConversationMetadata::LinkVector(const FString& VectorId)
{
    if (!VectorId.IsEmpty() && !LinkedVectorIds.Contains(VectorId))
    {
        LinkedVectorIds.Add(VectorId);
    }
}

void UConversationMetadata::UnlinkVector(const FString& VectorId)
{
    LinkedVectorIds.Remove(VectorId);
}

bool UConversationMetadata::HasVector(const FString& VectorId) const
{
    return LinkedVectorIds.Contains(VectorId);
}

