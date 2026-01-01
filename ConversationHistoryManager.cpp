#include "ConversationHistoryManager.h"

UConversationHistory* UConversationHistoryManager::CreateConversation(const FString& ConversationId, bool& bSuccess, FString& OutError)
{
    bSuccess = false;
    OutError.Reset();

    if (ConversationId.IsEmpty())
    {
        OutError = TEXT("Conversation ID is empty.");
        return nullptr;
    }

    if (Conversations.Contains(ConversationId))
    {
        OutError = FString::Printf(TEXT("Conversation ID %s already exists."), *ConversationId);
        return nullptr;
    }

    UConversationHistory* NewConv = NewObject<UConversationHistory>(this);
    if (!ensure(NewConv))
    {
        OutError = TEXT("Failed to create ConversationHistory object.");
        return nullptr;
    }
    NewConv->Initialize(ConversationId);
    Conversations.Add(ConversationId, NewConv);
    bSuccess = true;
    return NewConv;
}

UConversationHistory* UConversationHistoryManager::GetConversation(const FString& ConversationId, bool& bFound) const
{
    bFound = false;
    if (const TObjectPtr<UConversationHistory>* FoundPtr = Conversations.Find(ConversationId))
    {
        bFound = true;
        return FoundPtr->Get();
    }
    return nullptr;
}

bool UConversationHistoryManager::DeleteConversation(const FString& ConversationId)
{
    return Conversations.Remove(ConversationId) > 0;
}

void UConversationHistoryManager::ListConversations(TArray<FString>& OutIds) const
{
    Conversations.GetKeys(OutIds);
}

