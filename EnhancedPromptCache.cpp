#include "EnhancedPromptCache.h"

#include "HAL/PlatformTime.h"

FEnhancedPromptCache::FEnhancedPromptCache()
{
    LastUpdateTimeSeconds = FPlatformTime::Seconds();
}

void FEnhancedPromptCache::AddPrompt(const FString& Prompt, const TArray<float>& Embedding, float InitialPriority, float TTL)
{
    FPromptData Item;
    Item.Prompt = Prompt;
    Item.Embedding = Embedding;
    Item.Priority = InitialPriority;
    Item.TimeToLive = TTL;
    Cache.Add(MoveTemp(Item));
}

FString FEnhancedPromptCache::GetHighestPriorityPrompt()
{
    UpdatePriorities();

    int32 BestIndex = INDEX_NONE;
    float BestPriority = -FLT_MAX;
    for (int32 i = 0; i < Cache.Num(); ++i)
    {
        if (Cache[i].Priority > BestPriority)
        {
            BestPriority = Cache[i].Priority;
            BestIndex = i;
        }
    }

    return (BestIndex != INDEX_NONE) ? Cache[BestIndex].Prompt : FString();
}

void FEnhancedPromptCache::UpdatePriorities()
{
    const double Now = FPlatformTime::Seconds();
    float DeltaSeconds = static_cast<float>(Now - LastUpdateTimeSeconds);
    if (DeltaSeconds < 0.f)
    {
        DeltaSeconds = 0.f;
    }
    // Clamp excessively large delta to avoid wiping cache in one call
    DeltaSeconds = FMath::Min(DeltaSeconds, 5.0f);
    LastUpdateTimeSeconds = Now;

    for (FPromptData& P : Cache)
    {
        P.Priority -= DecayRate * DeltaSeconds;
        P.TimeToLive -= DeltaSeconds;
    }

    Cache.RemoveAll([](const FPromptData& P)
    {
        return (P.TimeToLive <= 0.f) || (P.Priority <= 0.f);
    });
}

void FEnhancedPromptCache::GetTopPrompts(int32 Count, TArray<FString>& OutPrompts)
{
    UpdatePriorities();
    OutPrompts.Reset();
    if (Cache.Num() == 0 || Count <= 0)
    {
        return;
    }

    TArray<int32> Indices; Indices.Reserve(Cache.Num());
    for (int32 i = 0; i < Cache.Num(); ++i) Indices.Add(i);

    Indices.Sort([this](int32 A, int32 B)
    {
        return Cache[A].Priority > Cache[B].Priority;
    });

    const int32 N = FMath::Min(Count, Indices.Num());
    OutPrompts.Reserve(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutPrompts.Add(Cache[Indices[i]].Prompt);
    }
}
