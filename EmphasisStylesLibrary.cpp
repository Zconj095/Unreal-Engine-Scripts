#include "EmphasisStylesLibrary.h"

FString UEmphasisStylesLibrary::ToLabel(EEmphasisStyle Style)
{
    switch (Style)
    {
    case EEmphasisStyle::AssociativeSpeaking:        return TEXT("Associative Speaking");
    case EEmphasisStyle::AttentiveSpeaking:          return TEXT("Attentive Speaking");
    case EEmphasisStyle::AutomaticSpeaking:          return TEXT("Automatic Speaking");
    case EEmphasisStyle::CasualSpeaking:             return TEXT("Casual Speaking");
    case EEmphasisStyle::CommunicativeSpeaking:      return TEXT("Communicative Speaking");
    case EEmphasisStyle::ConcernedSpeaking:          return TEXT("Concerned Speaking");
    case EEmphasisStyle::ConversationSpeaking:       return TEXT("Conversation Speaking");
    case EEmphasisStyle::CounteractiveSpeaking:      return TEXT("Counteractive Speaking");
    case EEmphasisStyle::CounterintuitiveSpeaking:   return TEXT("Counterintuitive Speaking");
    case EEmphasisStyle::DefensiveSpeaking:          return TEXT("Defensive Speaking");
    case EEmphasisStyle::DirectAndFocusedSpeaking:   return TEXT("Direct And Focused Speaking");
    case EEmphasisStyle::DirectAndPointedSpeaking:   return TEXT("Direct And Pointed Speaking");
    case EEmphasisStyle::DirectAndFocusedIntent:     return TEXT("Direct And Focused Intent");
    case EEmphasisStyle::DirectSpeaking:             return TEXT("Direct Speaking");
    case EEmphasisStyle::EmotionalSpeaking:          return TEXT("Emotional Speaking");
    case EEmphasisStyle::EmpathicSpeaking:           return TEXT("Empathic Speaking");
    case EEmphasisStyle::FormalSpeaking:             return TEXT("Formal Speaking");
    case EEmphasisStyle::GentleSpeaking:             return TEXT("Gentle Speaking");
    case EEmphasisStyle::HarmonicSpeaking:           return TEXT("Harmonic Speaking");
    case EEmphasisStyle::IntelligenceSpeaking:       return TEXT("Intelligence Speaking");
    case EEmphasisStyle::IntuitiveSpeaking:          return TEXT("Intuitive Speaking");
    case EEmphasisStyle::KnowledgeableSpeaking:      return TEXT("Knowledgeable Speaking");
    case EEmphasisStyle::LongAndDetailedSpeaking:    return TEXT("Long And Detailed Speaking");
    case EEmphasisStyle::LongAndDirective:           return TEXT("Long And Directive");
    case EEmphasisStyle::LongBreathed:               return TEXT("Long Breathed");
    case EEmphasisStyle::LongWinded:                 return TEXT("Long Winded");
    case EEmphasisStyle::LoudAndBooming:             return TEXT("Loud And Booming");
    case EEmphasisStyle::LoudAndSoftSpoken:          return TEXT("Loud And Soft Spoken");
    case EEmphasisStyle::ManualSpeaking:             return TEXT("Manual Speaking");
    case EEmphasisStyle::MelodicSpeaking:            return TEXT("Melodic Speaking");
    case EEmphasisStyle::ModestSpeaking:             return TEXT("Modest Speaking");
    case EEmphasisStyle::MotivatedSpeaking:          return TEXT("Motivated Speaking");
    case EEmphasisStyle::NeutralSpeaking:            return TEXT("Neutral Speaking");
    case EEmphasisStyle::PassiveSpeaking:            return TEXT("Passive Speaking");
    case EEmphasisStyle::PeriodicSpeech:             return TEXT("Periodic Speech");
    case EEmphasisStyle::PhilosophicalSpeaking:      return TEXT("Philosophical Speaking");
    case EEmphasisStyle::PositionedSpeaking:         return TEXT("Positioned Speaking");
    case EEmphasisStyle::PositiveSpeaking:           return TEXT("Positive Speaking");
    case EEmphasisStyle::PreciseAndToThePoint:       return TEXT("Precise And To The Point");
    case EEmphasisStyle::ProperSpeaking:             return TEXT("Proper Speaking");
    case EEmphasisStyle::QuietAndSoftSpokenSpeaking: return TEXT("Quiet And Soft Spoken Speaking");
    case EEmphasisStyle::SemiManualSpeaking:         return TEXT("Semi Manual Speaking");
    case EEmphasisStyle::SemiAutomaticSpeaking:      return TEXT("Semi Automatic Speaking");
    case EEmphasisStyle::SeriousSpeaking:            return TEXT("Serious Speaking");
    case EEmphasisStyle::SharpPitchedSpeaking:       return TEXT("Sharp Pitched Speaking");
    case EEmphasisStyle::ShortAndDirect:             return TEXT("Short And Direct");
    case EEmphasisStyle::ShortBurst:                 return TEXT("Short Burst");
    case EEmphasisStyle::ShortWinded:                return TEXT("Short Winded");
    case EEmphasisStyle::SoftSpoken:                 return TEXT("Soft Spoken");
    case EEmphasisStyle::SoftToned:                  return TEXT("Soft Toned");
    case EEmphasisStyle::SympathicSpeaking:          return TEXT("Sympathic Speaking");
    case EEmphasisStyle::VocalNegate:                return TEXT("Vocal Negate");
    default:                                         return TEXT("Wise Speaking");
    }
}

FString UEmphasisStylesLibrary::DefinitionOf(EEmphasisStyle Style)
{
    switch (Style)
    {
    case EEmphasisStyle::AssociativeSpeaking:
        return TEXT("A Form Of Emphasis That Associates Itself With Other Forms Of Speaking Or Emphasis");
    case EEmphasisStyle::AttentiveSpeaking:
        return TEXT("A Form Of Emphasis That Uses Or Has A Focus Of Specific Attention Towards The Speaking Patterns");
    case EEmphasisStyle::AutomaticSpeaking:
        return TEXT("A Form Of Emphasis That Forms With Automatic Vocals While Speaking");
    case EEmphasisStyle::CasualSpeaking:
        return TEXT("A Form Of Emphasis That Uses Casual Speaking Skills As A Priority While Speaking");
    case EEmphasisStyle::CommunicativeSpeaking:
        return TEXT("A Form Of Emphasis That Has Focus On Communication With The Voice");
    case EEmphasisStyle::ConcernedSpeaking:
        return TEXT("A Form Of Emphasis That Has An Intense Concerned Voice While Speaking");
    case EEmphasisStyle::ConversationSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On Having Conversations While Speaking");
    case EEmphasisStyle::CounteractiveSpeaking:
        return TEXT("A Form Of Emphasis That Has A Method Of Speaking That Forms From  Counteractive Responses While Speaking");
    case EEmphasisStyle::CounterintuitiveSpeaking:
        return TEXT("A Form Of Emphasis That Forms From Counterintuitive Responses While Speaking");
    case EEmphasisStyle::DefensiveSpeaking:
        return TEXT("A Form Of Emphasis Used With Speaking Methods That Are Used For Defense");
    case EEmphasisStyle::DirectAndFocusedSpeaking:
        return TEXT("A Form Of Emphasis Used To Get Directly To The Focused Circumstance Within The Conversation Without Losing Focus");
    case EEmphasisStyle::DirectAndPointedSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On The Direct Focal Points Of The Conversation");
    case EEmphasisStyle::DirectAndFocusedIntent:
        return TEXT("A Form Of Emphasis That Focuses On A Direct And Focused Intention And Or Direct Meaning");
    case EEmphasisStyle::DirectSpeaking:
        return TEXT("A Form Of Emphasis Focused On Directly Speaking To Someone Or Something");
    case EEmphasisStyle::EmotionalSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On Speaking With Emotions");
    case EEmphasisStyle::EmpathicSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On Speaking With Empathy");
    case EEmphasisStyle::FormalSpeaking:
        return TEXT("A Form Of Empathy That Forms With Formal Speaking And Communication Skills");
    case EEmphasisStyle::GentleSpeaking:
        return TEXT("A Form Of Emphasis That Forms With A Gentle Voice And Or Vocal Patterns");
    case EEmphasisStyle::HarmonicSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On The Formation Of Harmony While Speaking");
    case EEmphasisStyle::IntelligenceSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On Speaking With Intelligence");
    case EEmphasisStyle::IntuitiveSpeaking:
        return TEXT("A Form Of Emphasis That Forms From Intuition And Or Thought Patterns That Directly Influence Vocal Patterns From Intuition");
    case EEmphasisStyle::KnowledgeableSpeaking:
        return TEXT("A Form Of Emphasis That Forms From By Speaking With Knowledge");
    case EEmphasisStyle::LongAndDetailedSpeaking:
        return TEXT("A Form Of Emphasis That Happens With Long Winded Breathing Patterns And Detailed Speaking Patterns");
    case EEmphasisStyle::LongAndDirective:
        return TEXT("A Form Of Emphasis That Forms By Speaking For Longer Periods Of Time Without Stopping And Using Direct Forms Of Speaking While Speaking");
    case EEmphasisStyle::LongBreathed:
        return TEXT("A Form Of Emphasis That Uses Longer Forms Of Speaking With Deep Long Breathtaking");
    case EEmphasisStyle::LongWinded:
        return TEXT("A Form Of Emphasis That Forms While The Speaker Speaks For Long Periods Of Time Without Taking A Breath Or Periodic Stopping Point To Gather Air");
    case EEmphasisStyle::LoudAndBooming:
        return TEXT("A Form Of Emphasis That Uses Loud Speaking Patterns With An Intensity Of Vocal Bass");
    case EEmphasisStyle::LoudAndSoftSpoken:
        return TEXT("A Form Of Emphasis That Forms When Someone That Uses Loud Emphasis Patterns Speaks With Soft Spoken");
    case EEmphasisStyle::ManualSpeaking:
        return TEXT("A Form Of Emphasis That Forms From Manual Method Vocal Responses And Patterns");
    case EEmphasisStyle::MelodicSpeaking:
        return TEXT("A Form Of Emphasis That Uses Melodies While Speaking To Become Melodic");
    case EEmphasisStyle::ModestSpeaking:
        return TEXT("A Form Of Emphasis That Has A Form Of Modesty Included While Speaking");
    case EEmphasisStyle::MotivatedSpeaking:
        return TEXT("A Form Of Emphasis That Has A Specific Amount Of Motivated Intent Included While Speaking");
    case EEmphasisStyle::NeutralSpeaking:
        return TEXT("A Form Of Emphasis That Uses Both Positive And Negative Vocal Patterns While Not Making Action On Either Positive Or Negative Vocal Responses And Staying Neutral");
    case EEmphasisStyle::PassiveSpeaking:
        return TEXT("A Form Of Emphasis That Forms An Indirect Intent And Wide Perspective Approach While Speaking With Indirect Meaning Towards Someone");
    case EEmphasisStyle::PeriodicSpeech:
        return TEXT("A Form Of Speech Patterns That Have Intervals Of Speaking Periodically");
    case EEmphasisStyle::PhilosophicalSpeaking:
        return TEXT("A Form Of Emphasis That Has A Focus On Philosophy While Speaking");
    case EEmphasisStyle::PositionedSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On The Position Of The Vocals And What The Vocals Are Directed At");
    case EEmphasisStyle::PositiveSpeaking:
        return TEXT("A Form Of Emphasis That Has Focus Towards The Speaker Speaking Positively About Oneself Or About Others");
    case EEmphasisStyle::PreciseAndToThePoint:
        return TEXT("A Form Of Emphasis That Directly Focuses On The Subject And Or Topic And Focuses On The solution To A Problem Or Circumstance");
    case EEmphasisStyle::ProperSpeaking:
        return TEXT("A Form Of Emphasis That Forms With Proper Communication Patterns And Speaking Patterns");
    case EEmphasisStyle::QuietAndSoftSpokenSpeaking:
        return TEXT("A Form Of Emphasis That Forms With A Lower Volume Soft Spoken Pattern");
    case EEmphasisStyle::SemiManualSpeaking:
        return TEXT("A Form Of Emphasis That Forms With Manual Method Speaking Patterns With Partial Amount Of Automatic Method Speaking Patterns");
    case EEmphasisStyle::SemiAutomaticSpeaking:
        return TEXT("A Form Of Emphasis That Forms With Automatic Method Speaking Patterns With Partial Amount Of Manual Method Speaking Patterns");
    case EEmphasisStyle::SeriousSpeaking:
        return TEXT("A Form Of Emphasis That Focuses On Speaking With Serious Emotion And Direct Form Of Emphasis Patterns");
    case EEmphasisStyle::SharpPitchedSpeaking:
        return TEXT("A Form Of Emphasis That Uses A Sharp Pitch Vocal Pattern While Speaking");
    case EEmphasisStyle::ShortAndDirect:
        return TEXT("A Form Of Emphasis That Uses A Form Of Short And Precise Sentences While Speaking Directly");
    case EEmphasisStyle::ShortBurst:
        return TEXT("A Form Of Emphasis That Uses A Form Of Short Loud And Booming Vocal Patterns While Speaking");
    case EEmphasisStyle::ShortWinded:
        return TEXT("A Form Of Emphasis That Uses A Form Of Speaking That Uses Short Breathing Patterns Instead Of Long Breathing Patterns While Speaking");
    case EEmphasisStyle::SoftSpoken:
        return TEXT("A Form Of Emphasis That Uses A Form Of Speaking That Is Soft");
    case EEmphasisStyle::SoftToned:
        return TEXT("A Form Of Emphasis That Uses A Soft Tone While Speaking");
    case EEmphasisStyle::SympathicSpeaking:
        return TEXT("A Form Of Emphasis That Uses Speaking While Having Sympathetic Sounds Within The Tone Of The Voice");
    case EEmphasisStyle::VocalNegate:
        return TEXT("A Form Of Emphasis That Can Cancel Out The Forms Of Other Vocal Sounds That Have No Correlation To The Direct Form Of Emphasis And The Beliefs That Connect To The Emphasis Itself");
    case EEmphasisStyle::WiseSpeaking:
        return TEXT("A Form Of Emphasis That Forms With Wisdom And Clarity");
    }
    return FString();
}

TArray<EEmphasisStyle> UEmphasisStylesLibrary::AllEmphasisStyles()
{
    TArray<EEmphasisStyle> Out;
    Out.Reserve(53);
    Out.Add(EEmphasisStyle::AssociativeSpeaking);
    Out.Add(EEmphasisStyle::AttentiveSpeaking);
    Out.Add(EEmphasisStyle::AutomaticSpeaking);
    Out.Add(EEmphasisStyle::CasualSpeaking);
    Out.Add(EEmphasisStyle::CommunicativeSpeaking);
    Out.Add(EEmphasisStyle::ConcernedSpeaking);
    Out.Add(EEmphasisStyle::ConversationSpeaking);
    Out.Add(EEmphasisStyle::CounteractiveSpeaking);
    Out.Add(EEmphasisStyle::CounterintuitiveSpeaking);
    Out.Add(EEmphasisStyle::DefensiveSpeaking);
    Out.Add(EEmphasisStyle::DirectAndFocusedSpeaking);
    Out.Add(EEmphasisStyle::DirectAndPointedSpeaking);
    Out.Add(EEmphasisStyle::DirectAndFocusedIntent);
    Out.Add(EEmphasisStyle::DirectSpeaking);
    Out.Add(EEmphasisStyle::EmotionalSpeaking);
    Out.Add(EEmphasisStyle::EmpathicSpeaking);
    Out.Add(EEmphasisStyle::FormalSpeaking);
    Out.Add(EEmphasisStyle::GentleSpeaking);
    Out.Add(EEmphasisStyle::HarmonicSpeaking);
    Out.Add(EEmphasisStyle::IntelligenceSpeaking);
    Out.Add(EEmphasisStyle::IntuitiveSpeaking);
    Out.Add(EEmphasisStyle::KnowledgeableSpeaking);
    Out.Add(EEmphasisStyle::LongAndDetailedSpeaking);
    Out.Add(EEmphasisStyle::LongAndDirective);
    Out.Add(EEmphasisStyle::LongBreathed);
    Out.Add(EEmphasisStyle::LongWinded);
    Out.Add(EEmphasisStyle::LoudAndBooming);
    Out.Add(EEmphasisStyle::LoudAndSoftSpoken);
    Out.Add(EEmphasisStyle::ManualSpeaking);
    Out.Add(EEmphasisStyle::MelodicSpeaking);
    Out.Add(EEmphasisStyle::ModestSpeaking);
    Out.Add(EEmphasisStyle::MotivatedSpeaking);
    Out.Add(EEmphasisStyle::NeutralSpeaking);
    Out.Add(EEmphasisStyle::PassiveSpeaking);
    Out.Add(EEmphasisStyle::PeriodicSpeech);
    Out.Add(EEmphasisStyle::PhilosophicalSpeaking);
    Out.Add(EEmphasisStyle::PositionedSpeaking);
    Out.Add(EEmphasisStyle::PositiveSpeaking);
    Out.Add(EEmphasisStyle::PreciseAndToThePoint);
    Out.Add(EEmphasisStyle::ProperSpeaking);
    Out.Add(EEmphasisStyle::QuietAndSoftSpokenSpeaking);
    Out.Add(EEmphasisStyle::SemiManualSpeaking);
    Out.Add(EEmphasisStyle::SemiAutomaticSpeaking);
    Out.Add(EEmphasisStyle::SeriousSpeaking);
    Out.Add(EEmphasisStyle::SharpPitchedSpeaking);
    Out.Add(EEmphasisStyle::ShortAndDirect);
    Out.Add(EEmphasisStyle::ShortBurst);
    Out.Add(EEmphasisStyle::ShortWinded);
    Out.Add(EEmphasisStyle::SoftSpoken);
    Out.Add(EEmphasisStyle::SoftToned);
    Out.Add(EEmphasisStyle::SympathicSpeaking);
    Out.Add(EEmphasisStyle::VocalNegate);
    Out.Add(EEmphasisStyle::WiseSpeaking);
    return Out;
}

