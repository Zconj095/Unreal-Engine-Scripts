#include "InstructionParser.h"

#include "Internationalization/Regex.h"

void UInstructionParser::ParseInstructions(const FString& Input, TArray<FString>& OutInstructions) const
{
    OutInstructions.Reset();
    if (Input.IsEmpty())
    {
        return;
    }

    // Case-insensitive: (?i)
    // Delimiters: punctuation end + whitespace OR "and then" OR ", then"
    const FRegexPattern Pattern(TEXT("(?i)(?<=[\\.\\!\\?])\\s+|\\band\\s+then\\b|,\\s+then\\b"));
    FRegexMatcher Matcher(Pattern, Input);

    int32 Start = 0;
    while (Matcher.FindNext())
    {
        const int32 End = Matcher.GetMatchBeginning();
        if (End > Start)
        {
            FString Chunk = Input.Mid(Start, End - Start);
            Chunk.TrimStartAndEndInline();
            if (!Chunk.IsEmpty())
            {
                OutInstructions.Add(MoveTemp(Chunk));
            }
        }
        Start = Matcher.GetMatchEnding();
    }

    // Remainder
    if (Start < Input.Len())
    {
        FString Tail = Input.Mid(Start);
        Tail.TrimStartAndEndInline();
        if (!Tail.IsEmpty())
        {
            OutInstructions.Add(MoveTemp(Tail));
        }
    }
}
