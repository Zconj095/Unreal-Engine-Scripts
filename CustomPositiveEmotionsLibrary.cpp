#include "CustomPositiveEmotionsLibrary.h"

FString UCustomPositiveEmotionsLibrary::ToLabel(ECustomPositiveEmotion Emotion)
{
    switch (Emotion)
    {
    case ECustomPositiveEmotion::Courage:    return TEXT("Courage");
    case ECustomPositiveEmotion::Faith:      return TEXT("Faith");
    case ECustomPositiveEmotion::Gratitude:  return TEXT("Gratitude");
    case ECustomPositiveEmotion::Happiness:  return TEXT("Happiness");
    case ECustomPositiveEmotion::Hope:       return TEXT("Hope");
    case ECustomPositiveEmotion::Joy:        return TEXT("Joy");
    case ECustomPositiveEmotion::Love:       return TEXT("Love");
    case ECustomPositiveEmotion::Serene:     return TEXT("Serene");
    case ECustomPositiveEmotion::Serenity:   return TEXT("Serenity");
    case ECustomPositiveEmotion::Serious:    return TEXT("Serious");
    default:                                 return TEXT("Temperance");
    }
}

FString UCustomPositiveEmotionsLibrary::DefinitionOf(ECustomPositiveEmotion Emotion)
{
    switch (Emotion)
    {
    case ECustomPositiveEmotion::Courage:
        return TEXT("An Emotion That Describes As Having A Brave Feeling Or Strong Intense Courageous Reaction And Or Behavior And Or Response From Subtle And Or A Large Emotional Response");
    case ECustomPositiveEmotion::Faith:
        return TEXT("An Emotion That Describes A Strong Feeling Of Commitment And Power Formed From Ones Own Beliefs And Or Values And Or Virtues That Strengthen And Form From A Higher Source And Or From Oneself");
    case ECustomPositiveEmotion::Gratitude:
        return TEXT("An Emotion That Describes A Feeling Of Appreciation Or Thankfulness And Full Of Positive Feelings And Moods And Behaviors");
    case ECustomPositiveEmotion::Happiness:
        return TEXT("An Emotion That Describes A Sense Of Joy And Happy Feeling");
    case ECustomPositiveEmotion::Hope:
        return TEXT("An Emotion That Describes A Strong Sense Of Desire To Succeed Through Circumstances Where Possibility Is Low Or Below Average To Happen And The Emotion Becomes Stronger While Within That Circumstance");
    case ECustomPositiveEmotion::Joy:
        return TEXT("An Emotion Describing A Sense Of Overwhelming Satisfaction Formed From Satisfied And Happy Feelings");
    case ECustomPositiveEmotion::Love:
        return TEXT("An Emotion Describing A Feeling That Can Only Be Described By The Actions And Effects Of The Way The Person Expresses Their Moods And Or Emotions And Or Feelings And The Actions And Behaviors That Form From Them Within The Development Of Emotions Moods And Behaviors When They Create The Feelings Together");
    case ECustomPositiveEmotion::Serene:
        return TEXT("An Emotion Describing A Calm Temperant Feeling Mood Emotion And Feeling Happening Simultaneously At The Same Time");
    case ECustomPositiveEmotion::Serenity:
        return TEXT("An Emotion Describing A Sense Of Calm Understanding That Happens When Serene Emotions Moods And Behaviors Collect Together And Merge With Temperance And Calm Satisfaction");
    case ECustomPositiveEmotion::Serious:
        return TEXT("An Emotion Describing A Form Of Discipline And Focused Mindset That Is Considered Specific To The Topic And Or Way That Something Is Described");
    case ECustomPositiveEmotion::Temperance:
        return TEXT("Is A Warm Feeling Formed From Gentle And Peaceful Circumstances Or Surroundings");
    }
    return FString();
}

TArray<ECustomPositiveEmotion> UCustomPositiveEmotionsLibrary::AllPositiveEmotions()
{
    TArray<ECustomPositiveEmotion> Out;
    Out.Reserve(11);
    Out.Add(ECustomPositiveEmotion::Courage);
    Out.Add(ECustomPositiveEmotion::Faith);
    Out.Add(ECustomPositiveEmotion::Gratitude);
    Out.Add(ECustomPositiveEmotion::Happiness);
    Out.Add(ECustomPositiveEmotion::Hope);
    Out.Add(ECustomPositiveEmotion::Joy);
    Out.Add(ECustomPositiveEmotion::Love);
    Out.Add(ECustomPositiveEmotion::Serene);
    Out.Add(ECustomPositiveEmotion::Serenity);
    Out.Add(ECustomPositiveEmotion::Serious);
    Out.Add(ECustomPositiveEmotion::Temperance);
    return Out;
}

