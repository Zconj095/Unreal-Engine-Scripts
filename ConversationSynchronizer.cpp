#include "ConversationSynchronizer.h"

#include "Misc/Guid.h"

FString UConversationSynchronizer::StartNewConversation()
{
    if (!HistoryManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartNewConversation: HistoryManager not set."));
        return FString();
    }

    const FString ConversationId = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
    bool bSuccess = false; FString Err;
    HistoryManager->CreateConversation(ConversationId, bSuccess, Err);
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create conversation '%s': %s"), *ConversationId, *Err);
        return FString();
    }
    return ConversationId;
}

void UConversationSynchronizer::SynchronizeMessage(const FString& ConversationId, const FString& Sender, const FString& Message)
{
    if (!HistoryManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("SynchronizeMessage: HistoryManager not set."));
        return;
    }

    bool bFound = false;
    UConversationHistory* Conv = HistoryManager->GetConversation(ConversationId, bFound);
    if (!bFound || !Conv)
    {
        UE_LOG(LogTemp, Warning, TEXT("SynchronizeMessage: Conversation '%s' not found."), *ConversationId);
        return;
    }

    Conv->AddMessage(Sender, Message);
}

void UConversationSynchronizer::GetActiveConversations(TArray<FString>& OutConversationIds) const
{
    OutConversationIds.Reset();
    if (!HistoryManager)
    {
        return;
    }
    HistoryManager->ListConversations(OutConversationIds);
}

