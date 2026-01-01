#include "ConversationVectorizer.h"

#include "Misc/Crc.h"

void UConversationVectorizer::VectorizeMessage(const FConversationMessage& Message, TArray<float>& OutEmbedding) const
{
    OutEmbedding.Reset();
    if (!EmbeddingGenerator)
    {
        return;
    }
    const int32 Seed = static_cast<int32>(FCrc::StrCrc32(*Message.Message));
    EmbeddingGenerator->GenerateEmbedding(Seed, OutEmbedding);
}

void UConversationVectorizer::VectorizeConversation(const UConversationHistory* Conversation, TArray<FFloatArray>& OutEmbeddings) const
{
    OutEmbeddings.Reset();
    if (!EmbeddingGenerator || !Conversation)
    {
        return;
    }

    OutEmbeddings.Reserve(Conversation->Messages.Num());
    for (const FConversationMessage& Msg : Conversation->Messages)
    {
        TArray<float> Emb;
        VectorizeMessage(Msg, Emb);
        FFloatArray Wrap; Wrap.Values = MoveTemp(Emb);
        OutEmbeddings.Add(MoveTemp(Wrap));
    }
}

