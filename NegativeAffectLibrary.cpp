#include "NegativeAffectLibrary.h"

// Labels
FString UNegativeAffectLibrary::ToLabel_Emotion(ENegativeEmotion V)
{
    switch (V)
    {
    case ENegativeEmotion::Anger:      return TEXT("ANGER");
    case ENegativeEmotion::Sad:        return TEXT("SAD");
    case ENegativeEmotion::Upset:      return TEXT("UPSET");
    case ENegativeEmotion::Trauma:     return TEXT("TRAUMA");
    case ENegativeEmotion::Fear:       return TEXT("FEAR");
    case ENegativeEmotion::Regretful:  return TEXT("REGRETFUL");
    case ENegativeEmotion::Guilt:      return TEXT("GUILT");
    case ENegativeEmotion::Melancholy: return TEXT("MEALANCHOLY");
    case ENegativeEmotion::Desperate:  return TEXT("DESPARATE");
    default:                           return TEXT("BITTER");
    }
}

FString UNegativeAffectLibrary::ToLabel_Mood(ENegativeMood V)
{
    switch (V)
    {
    case ENegativeMood::Depressive: return TEXT("DEPRESSIVE");
    case ENegativeMood::Hopeless:   return TEXT("HOPELESS");
    case ENegativeMood::Shameful:   return TEXT("SHAMEFUL");
    case ENegativeMood::Hateful:    return TEXT("HATEFUL");
    case ENegativeMood::Anxious:    return TEXT("ANXIOUS");
    case ENegativeMood::Bitter:     return TEXT("BITTER");
    default:                        return TEXT("VENGEFUL");
    }
}

FString UNegativeAffectLibrary::ToLabel_Feeling(ENegativeFeeling V)
{
    switch (V)
    {
    case ENegativeFeeling::Sorrow:        return TEXT("SORROW");
    case ENegativeFeeling::Depressed:     return TEXT("DEPRESSED");
    case ENegativeFeeling::Hurt:          return TEXT("HURT");
    case ENegativeFeeling::Anguish:       return TEXT("ANGUISH");
    case ENegativeFeeling::Sadness:       return TEXT("SADNESS");
    case ENegativeFeeling::Upset:         return TEXT("UPSET");
    case ENegativeFeeling::Angry:         return TEXT("ANGRY");
    case ENegativeFeeling::Traumatic:     return TEXT("TRAUMATIC");
    case ENegativeFeeling::Disgust:       return TEXT("DISGUST");
    case ENegativeFeeling::Rejection:     return TEXT("REJECTION");
    case ENegativeFeeling::Torment:       return TEXT("TORMENT");
    case ENegativeFeeling::Envy:          return TEXT("ENVY");
    case ENegativeFeeling::Dejected:      return TEXT("DEJECTED");
    case ENegativeFeeling::Pity:          return TEXT("PITY");
    case ENegativeFeeling::Misery:        return TEXT("MISERY");
    case ENegativeFeeling::Agony:         return TEXT("AGONY");
    case ENegativeFeeling::GuiltF:        return TEXT("GUILT");
    case ENegativeFeeling::Hate:          return TEXT("HATE");
    case ENegativeFeeling::Lazy:          return TEXT("LAZY");
    case ENegativeFeeling::Envious:       return TEXT("ENVIOUS");
    case ENegativeFeeling::Jealous:       return TEXT("JEALOUS");
    case ENegativeFeeling::Regret:        return TEXT("REGRET");
    case ENegativeFeeling::Powerless:     return TEXT("POWERLESS");
    case ENegativeFeeling::Weak:          return TEXT("WEAK");
    case ENegativeFeeling::Gloomy:        return TEXT("GLOOMY");
    case ENegativeFeeling::DesperateF:    return TEXT("DESPERATE");
    case ENegativeFeeling::ShamefulF:     return TEXT("SHAMEFUL");
    case ENegativeFeeling::Irate:         return TEXT("IRRATE");
    case ENegativeFeeling::Irritation:    return TEXT("IRRITATION");
    case ENegativeFeeling::Frustration:   return TEXT("FRUSTRATION");
    case ENegativeFeeling::Fearful:       return TEXT("FEARFUL");
    case ENegativeFeeling::VengefulF:     return TEXT("VENGEFUL");
    case ENegativeFeeling::Distrust:      return TEXT("DISTRUST");
    case ENegativeFeeling::Disconnected:  return TEXT("DISCONNECTED");
    case ENegativeFeeling::Disbelief:     return TEXT("DISBELIEF");
    case ENegativeFeeling::Misunderstood: return TEXT("MISUNDERSTOOD");
    case ENegativeFeeling::Hollow:        return TEXT("HOLLOW");
    case ENegativeFeeling::Frightened:    return TEXT("FRIEGHTENED");
    case ENegativeFeeling::Fright:        return TEXT("FRIEGHT");
    case ENegativeFeeling::Frightful:     return TEXT("FRIEGHTFUL");
    case ENegativeFeeling::Uncontrolled:  return TEXT("UNCONTROLLED");
    default:                              return TEXT("NOTHINGNESS");
    }
}

// Definitions
FString UNegativeAffectLibrary::DefinitionOf_Emotion(ENegativeEmotion V)
{
    switch (V)
    {
    case ENegativeEmotion::Anger:
        return TEXT("An emotion forming large connections to disappointment and actions that may lead to bitter truth and/or vengeance.");
    case ENegativeEmotion::Sad:
        return TEXT("Experiences or connects to sorrow and upset emotions/feelings.");
    case ENegativeEmotion::Upset:
        return TEXT("Connects to disappointment and not being happy about something.");
    case ENegativeEmotion::Trauma:
        return TEXT("Formed from harsh past connections and experiences that affect the present moment.");
    case ENegativeEmotion::Fear:
        return TEXT("Formed from concern or worry that something can or will happen, or has happened.");
    case ENegativeEmotion::Regretful:
        return TEXT("Formed from having regret from an action, words, or circumstance that already happened.");
    case ENegativeEmotion::Guilt:
        return TEXT("Contains regrets and experiences from actions/words/events across emotions, moods, and feelings.");
    case ENegativeEmotion::Melancholy:
        return TEXT("Experienced from sadness, sorrow, guilt or trauma and depressive moods.");
    case ENegativeEmotion::Desperate:
        return TEXT("Formed from a need for action for something to happen or come into place.");
    case ENegativeEmotion::Bitter:
        return TEXT("Connected to strong harsh truths with dense emotional connections and understanding why/how they formed.");
    }
    return FString();
}

FString UNegativeAffectLibrary::DefinitionOf_Mood(ENegativeMood V)
{
    switch (V)
    {
    case ENegativeMood::Depressive: return TEXT("Contains negativity and lacks energy with connections to affects.");
    case ENegativeMood::Hopeless:   return TEXT("Where hope does not exist or is hollow/void.");
    case ENegativeMood::Shameful:   return TEXT("Ignores boundaries/restrictions and allows actions anyway.");
    case ENegativeMood::Hateful:    return TEXT("Holding a specific amount of hate.");
    case ENegativeMood::Anxious:    return TEXT("Containing worry and/or neglect.");
    case ENegativeMood::Bitter:     return TEXT("Containing many negative and strict actions.");
    case ENegativeMood::Vengeful:   return TEXT("Contains hate and uses it to produce actions from vengeance.");
    }
    return FString();
}

FString UNegativeAffectLibrary::DefinitionOf_Feeling(ENegativeFeeling V)
{
    switch (V)
    {
    case ENegativeFeeling::Sorrow:        return TEXT("Great sadness and upset feelings connected together.");
    case ENegativeFeeling::Depressed:     return TEXT("From pressure resonating between sadness and emotions tied to guilt/trauma while upset.");
    case ENegativeFeeling::Hurt:          return TEXT("Pain from connected affects and sensitivity to sensations.");
    case ENegativeFeeling::Anguish:       return TEXT("Pain and agony understood through connected experiences.");
    case ENegativeFeeling::Sadness:       return TEXT("Low emotional connection; little happiness exists.");
    case ENegativeFeeling::Upset:         return TEXT("Forms from lack of happiness within an area.");
    case ENegativeFeeling::Angry:         return TEXT("Directs action through anger instead of positive emotions.");
    case ENegativeFeeling::Traumatic:     return TEXT("Contains trauma and instances across time/timeframes.");
    case ENegativeFeeling::Disgust:       return TEXT("Non‑satisfaction with what happened or is happening.");
    case ENegativeFeeling::Rejection:     return TEXT("When something is refused or denied from happening.");
    case ENegativeFeeling::Torment:       return TEXT("Multiple forms of pain from harsh or repeated circumstances.");
    case ENegativeFeeling::Envy:          return TEXT("Connects pity and regret toward another's possession/state.");
    case ENegativeFeeling::Dejected:      return TEXT("Feeling rejected or not existing within a group or space.");
    case ENegativeFeeling::Pity:          return TEXT("Unsatisfying conditions leading to neglectful feeling.");
    case ENegativeFeeling::Misery:        return TEXT("Connection to existing as not happy.");
    case ENegativeFeeling::Agony:         return TEXT("From hurt and misery; can form from hate and pain.");
    case ENegativeFeeling::GuiltF:        return TEXT("Feeling connected to guilty emotions and their relations.");
    case ENegativeFeeling::Hate:          return TEXT("Capability to be without what is loved.");
    case ENegativeFeeling::Lazy:          return TEXT("Formed from a relaxed, inactive feeling.");
    case ENegativeFeeling::Envious:       return TEXT("Desire to have something another entity has using envy.");
    case ENegativeFeeling::Jealous:       return TEXT("Desires holding envy and regret of not having what another has.");
    case ENegativeFeeling::Regret:        return TEXT("Wishing something did not happen at a specific point.");
    case ENegativeFeeling::Powerless:     return TEXT("Cannot gain power/strength to do desired things.");
    case ENegativeFeeling::Weak:          return TEXT("From small, powerless connections.");
    case ENegativeFeeling::Gloomy:        return TEXT("Constant worry/neglect about past/present/possible events.");
    case ENegativeFeeling::DesperateF:    return TEXT("Final actions/series to make something happen correctly.");
    case ENegativeFeeling::ShamefulF:     return TEXT("Negative, not recognized actions containing shame.");
    case ENegativeFeeling::Irate:         return TEXT("Agitation and frustration and/or anger in some form.");
    case ENegativeFeeling::Irritation:    return TEXT("Agitation or upset affects influencing pain experience.");
    case ENegativeFeeling::Frustration:   return TEXT("Neglect of multiple words/actions happening repeatedly over time.");
    case ENegativeFeeling::Fearful:       return TEXT("Contains fear and worry within affects.");
    case ENegativeFeeling::VengefulF:     return TEXT("Contains hurt and vengeance forming action and words from pain.");
    case ENegativeFeeling::Distrust:      return TEXT("Decreased or absent trust.");
    case ENegativeFeeling::Disconnected:  return TEXT("Connections no longer exist that existed before.");
    case ENegativeFeeling::Disbelief:     return TEXT("More than one unproven belief present in existence/time.");
    case ENegativeFeeling::Misunderstood: return TEXT("Connections not understood correctly; considered not valid.");
    case ENegativeFeeling::Hollow:        return TEXT("Containing void with nothingness.");
    case ENegativeFeeling::Frightened:    return TEXT("Contained with fear and fright from worry.");
    case ENegativeFeeling::Fright:        return TEXT("Formed with fearful actions/responses.");
    case ENegativeFeeling::Frightful:     return TEXT("Containing fright and fear.");
    case ENegativeFeeling::Uncontrolled:  return TEXT("No control of actions, behaviors, affects.");
    case ENegativeFeeling::Nothingness:   return TEXT("From hollow and empty feelings that cannot be added to.");
    }
    return FString();
}

// Lists
TArray<ENegativeEmotion> UNegativeAffectLibrary::AllNegativeEmotions()
{
    return { ENegativeEmotion::Anger, ENegativeEmotion::Sad, ENegativeEmotion::Upset, ENegativeEmotion::Trauma, ENegativeEmotion::Fear, ENegativeEmotion::Regretful, ENegativeEmotion::Guilt, ENegativeEmotion::Melancholy, ENegativeEmotion::Desperate, ENegativeEmotion::Bitter };
}

TArray<ENegativeMood> UNegativeAffectLibrary::AllNegativeMoods()
{
    return { ENegativeMood::Depressive, ENegativeMood::Hopeless, ENegativeMood::Shameful, ENegativeMood::Hateful, ENegativeMood::Anxious, ENegativeMood::Bitter, ENegativeMood::Vengeful };
}

TArray<ENegativeFeeling> UNegativeAffectLibrary::AllNegativeFeelings()
{
    return {
        ENegativeFeeling::Sorrow,
        ENegativeFeeling::Depressed,
        ENegativeFeeling::Hurt,
        ENegativeFeeling::Anguish,
        ENegativeFeeling::Sadness,
        ENegativeFeeling::Upset,
        ENegativeFeeling::Angry,
        ENegativeFeeling::Traumatic,
        ENegativeFeeling::Disgust,
        ENegativeFeeling::Rejection,
        ENegativeFeeling::Torment,
        ENegativeFeeling::Envy,
        ENegativeFeeling::Dejected,
        ENegativeFeeling::Pity,
        ENegativeFeeling::Misery,
        ENegativeFeeling::Agony,
        ENegativeFeeling::GuiltF,
        ENegativeFeeling::Hate,
        ENegativeFeeling::Lazy,
        ENegativeFeeling::Envious,
        ENegativeFeeling::Jealous,
        ENegativeFeeling::Regret,
        ENegativeFeeling::Powerless,
        ENegativeFeeling::Weak,
        ENegativeFeeling::Gloomy,
        ENegativeFeeling::DesperateF,
        ENegativeFeeling::ShamefulF,
        ENegativeFeeling::Irate,
        ENegativeFeeling::Irritation,
        ENegativeFeeling::Frustration,
        ENegativeFeeling::Fearful,
        ENegativeFeeling::VengefulF,
        ENegativeFeeling::Distrust,
        ENegativeFeeling::Disconnected,
        ENegativeFeeling::Disbelief,
        ENegativeFeeling::Misunderstood,
        ENegativeFeeling::Hollow,
        ENegativeFeeling::Frightened,
        ENegativeFeeling::Fright,
        ENegativeFeeling::Frightful,
        ENegativeFeeling::Uncontrolled,
        ENegativeFeeling::Nothingness
    };
}

