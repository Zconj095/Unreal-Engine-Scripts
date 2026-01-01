#include "HumanNaturesLibrary.h"

FString UHumanNaturesLibrary::ToLabel(EHumanNature Nature)
{
    switch (Nature)
    {
    case EHumanNature::Adamant:  return TEXT("Adamant");
    case EHumanNature::Bashful:  return TEXT("Bashful");
    case EHumanNature::Bold:     return TEXT("Bold");
    case EHumanNature::Brave:    return TEXT("Brave");
    case EHumanNature::Calm:     return TEXT("Calm");
    case EHumanNature::Careful:  return TEXT("Careful");
    case EHumanNature::Docile:   return TEXT("Docile");
    case EHumanNature::Gentle:   return TEXT("Gentle");
    case EHumanNature::Hardy:    return TEXT("Hardy");
    case EHumanNature::Hasty:    return TEXT("Hasty");
    case EHumanNature::Impish:   return TEXT("Impish");
    case EHumanNature::Jolly:    return TEXT("Jolly");
    case EHumanNature::Lax:      return TEXT("Lax");
    case EHumanNature::Lonely:   return TEXT("Lonely");
    case EHumanNature::Mild:     return TEXT("Mild");
    case EHumanNature::Modest:   return TEXT("Modest");
    case EHumanNature::Naive:    return TEXT("Naive");
    case EHumanNature::Naughty:  return TEXT("Naughty");
    case EHumanNature::Quiet:    return TEXT("Quiet");
    case EHumanNature::Quirky:   return TEXT("Quirky");
    case EHumanNature::Rash:     return TEXT("Rash");
    case EHumanNature::Relaxed:  return TEXT("Relaxed");
    case EHumanNature::Sassy:    return TEXT("Sassy");
    case EHumanNature::Serious:  return TEXT("Serious");
    default:                     return TEXT("Timid");
    }
}

FString UHumanNaturesLibrary::DefinitionOf(EHumanNature Nature)
{
    switch (Nature)
    {
    case EHumanNature::Adamant:
        return TEXT("Stubbornly determined and unyielding in opinion or purpose, often focused and single-minded in pursuing goals.");
    case EHumanNature::Bashful:
        return TEXT("Shy or easily embarrassed, often avoiding attention or social interaction, and hesitant to put oneself forward.");
    case EHumanNature::Bold:
        return TEXT("Confident and courageous, willing to take risks and face challenges head-on, often with a fearless attitude.");
    case EHumanNature::Brave:
        return TEXT("Displays courage and willingness to face danger or adversity without showing fear, often taking on difficult tasks with resolve.");
    case EHumanNature::Calm:
        return TEXT("Serene and composed, not easily disturbed or excited, often maintaining a level-headed and peaceful demeanor in stressful situations.");
    case EHumanNature::Careful:
        return TEXT("Cautious and attentive to detail, often taking steps to avoid mistakes or danger, and considering potential risks before acting.");
    case EHumanNature::Docile:
        return TEXT("Gentle and easily managed or led, often compliant and cooperative, with a tendency to follow instructions without resistance.");
    case EHumanNature::Gentle:
        return TEXT("Kind and soft-hearted, with a tender and considerate approach to others, often avoiding harshness or aggression.");
    case EHumanNature::Hardy:
        return TEXT("Robust and resilient, capable of withstanding tough conditions or challenges, often showing endurance and strength.");
    case EHumanNature::Hasty:
        return TEXT("Quick to act or make decisions, often without fully considering the consequences, sometimes resulting in rash or impulsive behavior.");
    case EHumanNature::Impish:
        return TEXT("Playfully mischievous, with a tendency to cause trouble or behave in a teasing manner, often with a sense of humor.");
    case EHumanNature::Jolly:
        return TEXT("Cheerful and lighthearted, with a happy and optimistic outlook, often spreading joy and laughter to those around.");
    case EHumanNature::Lax:
        return TEXT("Relaxed and easy-going, often not strict or demanding, with a tendency to be lenient or unconcerned about details.");
    case EHumanNature::Lonely:
        return TEXT("Prefers solitude or feels isolated, often experiencing a sense of being alone or disconnected from others.");
    case EHumanNature::Mild:
        return TEXT("Gentle in nature and temperament, not prone to extreme reactions or behavior, often calm and soothing in demeanor.");
    case EHumanNature::Modest:
        return TEXT("Humble and unassuming, often downplaying one’s own achievements or abilities, with a preference for avoiding the spotlight.");
    case EHumanNature::Naive:
        return TEXT("Innocent and inexperienced, often showing a lack of worldly knowledge or sophistication, with a tendency to be easily trusting.");
    case EHumanNature::Naughty:
        return TEXT("Playfully disobedient or mischievous, often delighting in breaking rules or causing minor trouble for fun.");
    case EHumanNature::Quiet:
        return TEXT("Reserved and soft-spoken, often preferring to observe rather than participate in conversation, and comfortable in silence.");
    case EHumanNature::Quirky:
        return TEXT("Unconventional and eccentric, often displaying unique or odd behavior that sets one apart from others in an endearing way.");
    case EHumanNature::Rash:
        return TEXT("Prone to acting without thinking, often making hasty decisions or taking impulsive actions that can lead to unintended consequences.");
    case EHumanNature::Relaxed:
        return TEXT("Easy-going and calm, not easily stressed or worried, often taking things in stride and maintaining a laid-back attitude.");
    case EHumanNature::Sassy:
        return TEXT("Bold and lively, often with a sharp or cheeky attitude, unafraid to speak one's mind or challenge others with wit and humor.");
    case EHumanNature::Serious:
        return TEXT("Focused and thoughtful, often taking matters with gravity and not inclined to humor or lightheartedness, with a strong sense of responsibility.");
    case EHumanNature::Timid:
        return TEXT("Shy and easily frightened, often hesitant to take risks or step outside of comfort zones, with a tendency to avoid confrontation.");
    }
    return FString();
}

TArray<EHumanNature> UHumanNaturesLibrary::AllNatures()
{
    return {
        EHumanNature::Adamant,
        EHumanNature::Bashful,
        EHumanNature::Bold,
        EHumanNature::Brave,
        EHumanNature::Calm,
        EHumanNature::Careful,
        EHumanNature::Docile,
        EHumanNature::Gentle,
        EHumanNature::Hardy,
        EHumanNature::Hasty,
        EHumanNature::Impish,
        EHumanNature::Jolly,
        EHumanNature::Lax,
        EHumanNature::Lonely,
        EHumanNature::Mild,
        EHumanNature::Modest,
        EHumanNature::Naive,
        EHumanNature::Naughty,
        EHumanNature::Quiet,
        EHumanNature::Quirky,
        EHumanNature::Rash,
        EHumanNature::Relaxed,
        EHumanNature::Sassy,
        EHumanNature::Serious,
        EHumanNature::Timid
    };
}

