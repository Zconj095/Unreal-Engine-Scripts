#include "AttentionTextLibrary.h"

#include "Containers/UnrealString.h"

float UAttentionTextLibrary::ComputeAttentionScore(const FString& Input, const FString& Context, bool bCaseSensitive)
{
    TArray<FString> Keywords;
    // Split on whitespace; ignores empty tokens
    Context.ParseIntoArrayWS(Keywords);

    if (Keywords.Num() == 0)
    {
        return 0.0f;
    }

    const ESearchCase::Type CaseRule = bCaseSensitive ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;

    int32 MatchCount = 0;
    for (const FString& Word : Keywords)
    {
        if (!Word.IsEmpty() && Input.Contains(Word, CaseRule))
        {
            ++MatchCount;
        }
    }

    return static_cast<float>(MatchCount) / static_cast<float>(Keywords.Num());
}

