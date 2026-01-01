#include "ConversationHistory.h"

void UConversationHistory::Initialize(const FString& InConversationId)
{
    ConversationId = InConversationId;
}

void UConversationHistory::AddMessage(const FString& Sender, const FString& Message)
{
    FConversationMessage Msg;
    Msg.Sender = Sender;
    Msg.Message = Message;
    Msg.Timestamp = FDateTime::UtcNow();
    Messages.Add(Msg);
}
