#include "MultiInstructionParserLibrary.h"

#include "Internationalization/Regex.h"

static void SplitSentences(const FString& Text, TArray<FString>& Out)
{
    Out.Reset();
    if (Text.IsEmpty()) return;

    const FRegexPattern SentPat(TEXT("(?<=[\\.\\!\\?])\\s+"));
    FRegexMatcher M(SentPat, Text);
    int32 Start = 0;
    while (M.FindNext())
    {
        const int32 End = M.GetMatchBeginning();
        if (End > Start)
        {
            FString Chunk = Text.Mid(Start, End - Start);
            Chunk.TrimStartAndEndInline();
            if (!Chunk.IsEmpty()) Out.Add(MoveTemp(Chunk));
        }
        Start = M.GetMatchEnding();
    }
    if (Start < Text.Len())
    {
        FString Tail = Text.Mid(Start);
        Tail.TrimStartAndEndInline();
        if (!Tail.IsEmpty()) Out.Add(MoveTemp(Tail));
    }
}

void UMultiInstructionParserLibrary::ParseInstructionSequences(const FString& Input, TArray<FStrArray>& OutSequences)
{
    OutSequences.Reset();
    if (Input.IsEmpty())
    {
        return;
    }

    // Case-insensitive connectors: then | next | and
    const FRegexPattern ConnPat(TEXT("(?i)\\b(?:then|next|and)\\b"));
    FRegexMatcher Mat(ConnPat, Input);

    int32 Start = 0;
    while (Mat.FindNext())
    {
        const int32 End = Mat.GetMatchBeginning();
        if (End > Start)
        {
            FString Seq = Input.Mid(Start, End - Start);
            Seq.TrimStartAndEndInline();
            if (!Seq.IsEmpty())
            {
                TArray<FString> Steps;
                SplitSentences(Seq, Steps);
                Steps.RemoveAll([](const FString& S){ return S.TrimStartAndEnd().IsEmpty(); });
                if (Steps.Num() > 0)
                {
                    FStrArray Wrap; Wrap.Values = MoveTemp(Steps);
                    OutSequences.Add(MoveTemp(Wrap));
                }
            }
        }
        Start = Mat.GetMatchEnding();
    }

    // Remainder
    if (Start < Input.Len())
    {
        FString Seq = Input.Mid(Start);
        Seq.TrimStartAndEndInline();
        if (!Seq.IsEmpty())
        {
            TArray<FString> Steps;
            SplitSentences(Seq, Steps);
            Steps.RemoveAll([](const FString& S){ return S.TrimStartAndEnd().IsEmpty(); });
            if (Steps.Num() > 0)
            {
                FStrArray Wrap; Wrap.Values = MoveTemp(Steps);
                OutSequences.Add(MoveTemp(Wrap));
            }
        }
    }
}
