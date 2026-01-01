#include "EnergyUnderstandingLibrary.h"

namespace
{
    static FORCEINLINE bool IsVowel(TCHAR C)
    {
        const TCHAR Up = FChar::ToUpper(C);
        return (Up == 'A' || Up == 'E' || Up == 'I' || Up == 'O' || Up == 'U' || Up == 'Y');
    }

    static FORCEINLINE int32 LetterIndexAZ(TCHAR C)
    {
        if (FChar::IsAlpha(C))
        {
            const TCHAR Up = FChar::ToUpper(C);
            return (int32(Up) - int32('A'));
        }
        return -1;
    }

    static int32 LetterWeight(TCHAR C, ELetterWeightScheme Scheme)
    {
        const int32 Idx = LetterIndexAZ(C);
        if (Idx < 0) return 0;
        switch (Scheme)
        {
        case ELetterWeightScheme::A1Z26: return Idx + 1; // A=1..Z=26
        case ELetterWeightScheme::ASCII: return (int32)FChar::ToUpper(C);
        default: return Idx + 1;
        }
    }
}

FString UEnergyUnderstandingLibrary::ToLabel_LetterWeights(ELetterWeightScheme Scheme)
{
    switch (Scheme)
    {
    case ELetterWeightScheme::A1Z26: return TEXT("A1Z26 (A=1..Z=26)");
    default:                         return TEXT("ASCII Codepoint");
    }
}

FString UEnergyUnderstandingLibrary::ToLabel_EnergyOrigin(EEnergyOrigin Origin)
{
    switch (Origin)
    {
    case EEnergyOrigin::Chemical:        return TEXT("Chemical");
    case EEnergyOrigin::Electrical:      return TEXT("Electrical");
    case EEnergyOrigin::Mechanical:      return TEXT("Mechanical");
    case EEnergyOrigin::Thermal:         return TEXT("Thermal");
    case EEnergyOrigin::Radiant:         return TEXT("Radiant (Light/EM)");
    case EEnergyOrigin::Nuclear:         return TEXT("Nuclear");
    case EEnergyOrigin::Gravitational:   return TEXT("Gravitational");
    case EEnergyOrigin::Elastic:         return TEXT("Elastic");
    case EEnergyOrigin::Sound:           return TEXT("Sound (Acoustic)");
    case EEnergyOrigin::Biological:      return TEXT("Biological (Metabolic)");
    case EEnergyOrigin::Environmental:   return TEXT("Environmental");
    case EEnergyOrigin::SocialEmotional: return TEXT("Social/Emotional");
    default:                             return TEXT("Spiritual");
    }
}

FString UEnergyUnderstandingLibrary::DefinitionOf_EnergyOrigin(EEnergyOrigin Origin)
{
    switch (Origin)
    {
    case EEnergyOrigin::Chemical:
        return TEXT("Energy released or stored in chemical bonds (e.g., metabolism, fuel).");
    case EEnergyOrigin::Electrical:
        return TEXT("Energy from electric charges and currents (potential and kinetic forms).");
    case EEnergyOrigin::Mechanical:
        return TEXT("Energy of motion and forces, including kinetic and potential in mechanical systems.");
    case EEnergyOrigin::Thermal:
        return TEXT("Energy associated with temperature and microscopic motion of particles (heat).");
    case EEnergyOrigin::Radiant:
        return TEXT("Electromagnetic energy such as light, radio, infrared, ultraviolet, X-ray.");
    case EEnergyOrigin::Nuclear:
        return TEXT("Energy from nuclear fission/fusion and binding energies in atomic nuclei.");
    case EEnergyOrigin::Gravitational:
        return TEXT("Potential energy due to gravitational fields.");
    case EEnergyOrigin::Elastic:
        return TEXT("Energy stored by deformation of materials (springs, elastic tissues).");
    case EEnergyOrigin::Sound:
        return TEXT("Energy carried by pressure waves in a medium (acoustic).");
    case EEnergyOrigin::Biological:
        return TEXT("Metabolic energy production and storage in living systems.");
    case EEnergyOrigin::Environmental:
        return TEXT("Contextual sources like wind, tides, sun exposure, ambient conditions.");
    case EEnergyOrigin::SocialEmotional:
        return TEXT("Conceptual energy from human interaction, motivation, and emotion.");
    case EEnergyOrigin::Spiritual:
        return TEXT("Belief-centered or spiritual notions of energy and meaning.");
    }
    return FString();
}

FTextEnergyStats UEnergyUnderstandingLibrary::ComputeTextEnergyStats(const FString& Text, ELetterWeightScheme Scheme)
{
    FTextEnergyStats Out;
    Out.Characters = Text.Len();
    Out.LetterHistogram.Init(0, 26);

    int32 Letters = 0, Digits = 0, Punct = 0, Vowels = 0, Consonants = 0;
    double EnergySum = 0.0;

    for (int32 i = 0; i < Text.Len(); ++i)
    {
        const TCHAR C = Text[i];
        if (FChar::IsAlpha(C))
        {
            Letters++;
            const int32 idx = LetterIndexAZ(C);
            if (idx >= 0) { Out.LetterHistogram[idx] += 1; }
            if (IsVowel(C)) Vowels++; else Consonants++;
            EnergySum += (double)LetterWeight(C, Scheme);
        }
        else if (FChar::IsDigit(C))
        {
            Digits++;
        }
        else if (!FChar::IsWhitespace(C))
        {
            Punct++;
        }
    }

    // Words
    TArray<FString> WordsArray;
    Text.ParseIntoArrayWS(WordsArray);
    Out.Words = WordsArray.Num();
    int32 LettersInWords = 0;
    Out.WordEnergyScores.Reserve(Out.Words);
    TMap<FString, int32> Freq;
    TSet<FString> Unique;
    for (FString& W : WordsArray)
    {
        FString Lower = W.ToLower();
        Unique.Add(Lower);
        Freq.FindOrAdd(Lower) += 1;

        double WEnergy = 0.0;
        for (int32 j = 0; j < W.Len(); ++j)
        {
            const TCHAR c = W[j];
            if (FChar::IsAlpha(c))
            {
                LettersInWords++;
                WEnergy += (double)LetterWeight(c, Scheme);
            }
        }
        Out.WordEnergyScores.Add(WEnergy);
    }

    // Most frequent word
    int32 BestCount = 0;
    FString BestWord;
    for (const TPair<FString,int32>& P : Freq)
    {
        if (P.Value > BestCount)
        {
            BestCount = P.Value;
            BestWord = P.Key;
        }
    }
    Out.MostFrequentWord = BestWord;

    Out.Letters = Letters;
    Out.Digits = Digits;
    Out.Punctuation = Punct;
    Out.UniqueWords = Unique.Num();
    Out.AverageWordLength = (Out.Words > 0) ? float(LettersInWords) / float(Out.Words) : 0.f;
    Out.VowelRatio = (Letters > 0) ? float(Vowels) / float(Letters) : 0.f;
    Out.ConsonantRatio = (Letters > 0) ? float(Consonants) / float(Letters) : 0.f;
    Out.StoredEnergyScore = EnergySum;
    return Out;
}

FVocalEnergyStats UEnergyUnderstandingLibrary::ComputeVocalEnergyStats(const FString& Transcript, float DurationSeconds, ELetterWeightScheme Scheme)
{
    FVocalEnergyStats Out;
    Out.DurationSeconds = FMath::Max(0.f, DurationSeconds);
    const FTextEnergyStats TextStats = ComputeTextEnergyStats(Transcript, Scheme);

    Out.StoredEnergyScore = TextStats.StoredEnergyScore;
    Out.WordsPerMinute = (Out.DurationSeconds > 0.f) ? (60.f * float(TextStats.Words) / Out.DurationSeconds) : 0.f;
    Out.CharsPerSecond = (Out.DurationSeconds > 0.f) ? (float(TextStats.Characters) / Out.DurationSeconds) : 0.f;

    // Very rough syllable estimate: count vowel groups in words
    int32 Syllables = 0;
    TArray<FString> WordsArray; Transcript.ParseIntoArrayWS(WordsArray);
    for (const FString& W : WordsArray)
    {
        bool InVowel = false;
        for (int32 i = 0; i < W.Len(); ++i)
        {
            const TCHAR c = W[i];
            const bool v = IsVowel(c);
            if (v && !InVowel) { Syllables++; InVowel = true; }
            else if (!v) { InVowel = false; }
        }
    }
    Out.SyllableEstimate = Syllables;
    Out.EnergyPerSecond = (Out.DurationSeconds > 0.f) ? (Out.StoredEnergyScore / double(Out.DurationSeconds)) : 0.0;

    Out.Summary = FString::Printf(TEXT("WPM=%.2f, CPS=%.2f, Syllables=%d, Energy=%.1f, Energy/s=%.2f"),
                                  Out.WordsPerMinute, Out.CharsPerSecond, Out.SyllableEstimate, Out.StoredEnergyScore, Out.EnergyPerSecond);
    return Out;
}

