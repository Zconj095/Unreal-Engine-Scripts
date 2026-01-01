#include "CustomMoodsLibrary.h"

FString UCustomMoodsLibrary::ToLabel(ECustomMood Mood)
{
    switch (Mood)
    {
    case ECustomMood::Calm:      return TEXT("Calm");
    case ECustomMood::Cheerful:  return TEXT("Cheerful");
    case ECustomMood::Content:   return TEXT("Content");
    case ECustomMood::Defensive: return TEXT("Defensive");
    case ECustomMood::Energetic: return TEXT("Energetic");
    case ECustomMood::Excited:   return TEXT("Excited");
    case ECustomMood::Grateful:  return TEXT("Grateful");
    case ECustomMood::Joyful:    return TEXT("Joyful");
    case ECustomMood::Loud:      return TEXT("Loud");
    case ECustomMood::Mellow:    return TEXT("Mellow");
    case ECustomMood::Narrative: return TEXT("Narrative");
    case ECustomMood::Neutral:   return TEXT("Neutral");
    case ECustomMood::Peaceful:  return TEXT("Peaceful");
    case ECustomMood::Quiet:     return TEXT("Quiet");
    case ECustomMood::Relaxed:   return TEXT("Relaxed");
    default:                     return TEXT("Talkative");
    }
}

FString UCustomMoodsLibrary::DefinitionOf(ECustomMood Mood)
{
    switch (Mood)
    {
    case ECustomMood::Calm:
        return TEXT("A Mood That Described A Form Of Inner Content And Peaceful Behavior Or Action");
    case ECustomMood::Cheerful:
        return TEXT("A Mood That Describes A Happy And Full Of Cheer Behavior Or Circumstance");
    case ECustomMood::Content:
        return TEXT("A Mood That Describes Peaceful And Relaxed Reactions Behaviors Or Actions");
    case ECustomMood::Defensive:
        return TEXT("A Mood That Described As Showing Defensive Or Guarded Behaviors Or Actions");
    case ECustomMood::Energetic:
        return TEXT("A Mood That Describes As Full Of Energy And Producing Action On That Energy Or Described As Energetic To A Vast Amount And Making Actions From The Energy Itself");
    case ECustomMood::Excited:
        return TEXT("A Mood That Forms Behaviors Or Actions From Excitement");
    case ECustomMood::Grateful:
        return TEXT("Action And Or Decisions Made From Thankfulness And Or Pleasure From Something Happening");
    case ECustomMood::Joyful:
        return TEXT("A Mood Described As Actions Forming From A Energy Full Of Joy");
    case ECustomMood::Loud:
        return TEXT("A Described Mood When Actions And Decisions Are Made From Loud Sounds Or Effects");
    case ECustomMood::Mellow:
        return TEXT("A Described Mood That Is More Relaxed And Considered Calm And Gentle");
    case ECustomMood::Narrative:
        return TEXT("A Mood Described By Speaking With A Specific Form Of Made Emphasis Or Emotions And Or Moods And Or Feelings");
    case ECustomMood::Neutral:
        return TEXT("A Mood That Describes Action That Is Not Positive Or Negative");
    case ECustomMood::Peaceful:
        return TEXT("A Mood That Describes A Content And Relaxed Action Or Behavior That Is Specific To The Form Of Peaceful Behavior From That Specific Person");
    case ECustomMood::Quiet:
        return TEXT("A Mood That Forms From Soft Or Low Volume Actions And Or Reactions And Or Behaviors");
    case ECustomMood::Relaxed:
        return TEXT("A Mood That Describes A Actions Made With A Released Amount Of Tension And Calm Feeling Or Approach For Moods And Or Behaviors And Or Also Emotions And Feelings With The Mood Itself");
    case ECustomMood::Talkative:
        return TEXT("A Mood That Describes Actions Made From Very Energetic Reactions Or Behaviors From Speech And Or Vocal Responses");
    }
    return FString();
}

TArray<ECustomMood> UCustomMoodsLibrary::AllMoods()
{
    TArray<ECustomMood> Out;
    Out.Reserve(16);
    Out.Add(ECustomMood::Calm);
    Out.Add(ECustomMood::Cheerful);
    Out.Add(ECustomMood::Content);
    Out.Add(ECustomMood::Defensive);
    Out.Add(ECustomMood::Energetic);
    Out.Add(ECustomMood::Excited);
    Out.Add(ECustomMood::Grateful);
    Out.Add(ECustomMood::Joyful);
    Out.Add(ECustomMood::Loud);
    Out.Add(ECustomMood::Mellow);
    Out.Add(ECustomMood::Narrative);
    Out.Add(ECustomMood::Neutral);
    Out.Add(ECustomMood::Peaceful);
    Out.Add(ECustomMood::Quiet);
    Out.Add(ECustomMood::Relaxed);
    Out.Add(ECustomMood::Talkative);
    return Out;
}

