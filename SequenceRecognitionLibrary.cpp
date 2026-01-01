#include "SequenceRecognitionLibrary.h"
#include "Internationalization/Regex.h"

namespace
{
    struct FParsedItem
    {
        int32 MainNumber = INDEX_NONE; // extracted number or roman
        int32 SecondaryNumber = INDEX_NONE; // e.g., episode in SxxExx
        FString Base;
        bool bMatched = false;
    };

    static int32 RomanToInt(const FString& Roman)
    {
        TMap<TCHAR,int32> Map{{'I',1},{'V',5},{'X',10},{'L',50},{'C',100},{'D',500},{'M',1000}};
        int32 Sum=0, Prev=0;
        for (int32 i=Roman.Len()-1;i>=0;--i)
        {
            const TCHAR c = FChar::ToUpper(Roman[i]);
            const int32 v = Map.Contains(c)?Map[c]:0;
            if (v<Prev) Sum-=v; else { Sum+=v; Prev=v; }
        }
        return Sum>0?Sum:INDEX_NONE;
    }

    static FString TrimNumberTokens(const FString& S)
    {
        FString Out = S;
        Out.TrimStartAndEndInline();
        // Remove trailing separators and numbers/roman tokens
        FRegexPattern P1(TEXT("^(.*?)(?:\\s*[-:]*\\s*(?:Book|Ebook|Movie|Music|Playlist|Title|Header|Vol(?:ume)?|Part|Chapter|Episode)\\s*)?(?:\\d+|[IVXLCDM]+)?\\s*$"), ERegexPatternFlags::CaseInsensitive);
        FRegexMatcher M(P1, Out);
        if (M.FindNext())
        {
            FString Prefix = M.GetCaptureGroup(1);
            Prefix.TrimStartAndEndInline();
            return Prefix;
        }
        return Out;
    }

    static FParsedItem ParseTitle(const FString& Title)
    {
        FParsedItem R; R.Base = TrimNumberTokens(Title);
        // Try SxxExx pattern first
        {
            FRegexPattern Pat(TEXT("(?i)S(\\d{1,2})E(\\d{1,2})"));
            FRegexMatcher Mat(Pat, Title);
            if (Mat.FindNext())
            {
                R.MainNumber = FCString::Atoi(*Mat.GetCaptureGroup(1));
                R.SecondaryNumber = FCString::Atoi(*Mat.GetCaptureGroup(2));
                R.bMatched = true; return R;
            }
        }
        // Try explicit tokens + number (Book 1, Vol 2, Part IV)
        {
            FRegexPattern Pat(TEXT("(?i)(?:Book|Ebook|Movie|Music|Playlist|Title|Header|Vol(?:ume)?|Part|Chapter|Episode)\\s*([0-9]+|[IVXLCDM]+)"));
            FRegexMatcher Mat(Pat, Title);
            if (Mat.FindNext())
            {
                const FString tok = Mat.GetCaptureGroup(1);
                int32 n = tok.IsNumeric() ? FCString::Atoi(*tok) : RomanToInt(tok);
                if (n>0) { R.MainNumber = n; R.bMatched = true; return R; }
            }
        }
        // Try trailing number
        {
            FRegexPattern Pat(TEXT("(?i)(?:^|\\s)([0-9]+)$"));
            FRegexMatcher Mat(Pat, Title);
            if (Mat.FindNext())
            {
                R.MainNumber = FCString::Atoi(*Mat.GetCaptureGroup(1));
                R.bMatched = true; return R;
            }
        }
        // Try trailing roman
        {
            FRegexPattern Pat(TEXT("(?i)\\b([IVXLCDM]+)$"));
            FRegexMatcher Mat(Pat, Title);
            if (Mat.FindNext())
            {
                const int32 n = RomanToInt(Mat.GetCaptureGroup(1));
                if (n>0) { R.MainNumber = n; R.bMatched = true; return R; }
            }
        }
        return R;
    }

    static FString InferPattern(const TArray<FParsedItem>& Items)
    {
        // Prefer SxxExx if many have secondary
        int32 WithEp = 0; for (const auto& I:Items) if (I.SecondaryNumber!=INDEX_NONE) ++WithEp;
        if (WithEp * 2 >= Items.Num()) return TEXT("S%02dE%02d");
        // Otherwise generic number
        return TEXT("%s %d");
    }
}

FString USequenceRecognitionLibrary::ToLabel_Category(ESequenceCategory Category)
{
    switch (Category)
    {
    case ESequenceCategory::BookTitle:      return TEXT("Book Title Sequences");
    case ESequenceCategory::EbookTitle:     return TEXT("Ebook Title Sequences");
    case ESequenceCategory::Genre:          return TEXT("Genre Sequences");
    case ESequenceCategory::Header:         return TEXT("Header Sequences");
    case ESequenceCategory::MovieTitle:     return TEXT("Movie Title Sequences");
    case ESequenceCategory::MusicTitle:     return TEXT("Music Title Sequences");
    case ESequenceCategory::Playlist:       return TEXT("Playlist Sequences");
    case ESequenceCategory::PlaylistTitle:  return TEXT("Playlist Title Sequences");
    case ESequenceCategory::TitleAndHeader: return TEXT("Title And Header Sequences");
    default:                                return TEXT("Title Sequences");
    }
}

FSequenceRecognitionReport USequenceRecognitionLibrary::RecognizeSequence(const TArray<FString>& Titles, ESequenceCategory Category)
{
    FSequenceRecognitionReport Report; Report.ItemCount = Titles.Num();
    if (Titles.Num() == 0) return Report;

    // Parse each title
    TArray<FParsedItem> Items; Items.Reserve(Titles.Num());
    for (const FString& T : Titles) Items.Add(ParseTitle(T));

    // Count matches and build base prefix heuristic (most common base)
    TMap<FString,int32> BaseFreq;
    int32 Matched = 0; for (const auto& I:Items){ if (I.bMatched) ++Matched; BaseFreq.FindOrAdd(I.Base)++; }
    Report.MatchedCount = Matched;

    // Determine common base
    int32 BestCount = 0; FString BestBase;
    for (const auto& Pair : BaseFreq) { if (Pair.Value > BestCount) { BestCount = Pair.Value; BestBase = Pair.Key; } }
    Report.BaseTitle = BestBase;

    // Order check (by main number or by SxxExx)
    int32 InOrder = 0; int32 Prev = -9999;
    for (const auto& I : Items)
    {
        if (!I.bMatched) continue;
        const int32 Key = (I.SecondaryNumber!=INDEX_NONE) ? (I.MainNumber*100 + I.SecondaryNumber) : I.MainNumber;
        if (Key >= Prev) { ++InOrder; Prev = Key; } else { Prev = Key; }
    }
    Report.InOrderCount = InOrder;

    // Anomalies: collect non-matching titles
    for (int32 idx=0; idx<Titles.Num(); ++idx) if (!Items[idx].bMatched) Report.Anomalies.Add(Titles[idx]);

    // Confidence: based on coverage and order
    const float Coverage = float(Matched) / float(Titles.Num());
    const float OrderScore = (Matched>0) ? float(InOrder) / float(Matched) : 0.f;
    Report.Confidence = 0.6f * Coverage + 0.4f * OrderScore;
    Report.bIsSequence = (Report.Confidence >= 0.5f && Matched >= 2);
    Report.DetectedPattern = InferPattern(Items);
    return Report;
}

FSequenceRecognitionReport USequenceRecognitionLibrary::RecognizeSequenceAuto(const TArray<FString>& Titles, ESequenceCategory& OutBestCategory)
{
    float Best = -1.f; FSequenceRecognitionReport BestRep; OutBestCategory = ESequenceCategory::Title;
    for (int i=0; i< (int)ESequenceCategory::Title + 1; ++i)
    {
        ESequenceCategory Cat = static_cast<ESequenceCategory>(i);
        const FSequenceRecognitionReport R = RecognizeSequence(Titles, Cat);
        if (R.Confidence > Best)
        {
            Best = R.Confidence; BestRep = R; OutBestCategory = Cat;
        }
    }
    return BestRep;
}
