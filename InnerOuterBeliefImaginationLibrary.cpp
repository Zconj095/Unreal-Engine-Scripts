#include "InnerOuterBeliefImaginationLibrary.h"

FString UInnerOuterBeliefImaginationLibrary::ToLabel(EInnerOuterBelief V)
{
    switch (V)
    {
    case EInnerOuterBelief::InnerBeliefs: return TEXT("Inner Beliefs");
    default:                               return TEXT("Outer Beliefs");
    }
}

FString UInnerOuterBeliefImaginationLibrary::ToLabelEmotion(EInnerOuterEmotion V)
{
    switch (V)
    {
    case EInnerOuterEmotion::InnerEmotions: return TEXT("Inner Emotions");
    default:                                  return TEXT("Outer Emotions");
    }
}

FString UInnerOuterBeliefImaginationLibrary::ToLabelFeeling(EInnerOuterFeeling V)
{
    switch (V)
    {
    case EInnerOuterFeeling::InnerFeelings: return TEXT("Inner Feelings");
    default:                                 return TEXT("Outer Feelings");
    }
}

FString UInnerOuterBeliefImaginationLibrary::ToLabelImagination(EInnerOuterImagination V)
{
    switch (V)
    {
    case EInnerOuterImagination::InnerImagination: return TEXT("Inner Imagination");
    default:                                       return TEXT("Outer Imagination");
    }
}

FString UInnerOuterBeliefImaginationLibrary::DefinitionOf(EInnerOuterBelief V)
{
    switch (V)
    {
    case EInnerOuterBelief::InnerBeliefs:
        return TEXT("Are Beliefs That Connect To The Inner Mind And/Or Brain And How Beliefs Are Connected Within");
    case EInnerOuterBelief::OuterBeliefs:
        return TEXT("Are Beliefs That Are Described From Experiences And/Or Experienced Visualizations And/Or Sights As Well As Formed From The Area Within");
    }
    return FString();
}

FString UInnerOuterBeliefImaginationLibrary::DefinitionOfEmotion(EInnerOuterEmotion V)
{
    switch (V)
    {
    case EInnerOuterEmotion::InnerEmotions:
        return TEXT("Are Emotions That Are Formed From The Chakras Themselves");
    case EInnerOuterEmotion::OuterEmotions:
        return TEXT("Are Emotions That Are Formed From The Auras Specifically");
    }
    return FString();
}

FString UInnerOuterBeliefImaginationLibrary::DefinitionOfFeeling(EInnerOuterFeeling V)
{
    switch (V)
    {
    case EInnerOuterFeeling::InnerFeelings:
        return TEXT("Are Feelings That Form From The Emotions Moods And Behaviors Of The Body Itself And The Emotions That Form Them");
    case EInnerOuterFeeling::OuterFeelings:
        return TEXT("Are Feelings That Form From External Reactions And/Or Experiences From The Body And How It Is Felt By Its Own Experiences Both Inner And Outer By Haptic Responses And Sensations");
    }
    return FString();
}

FString UInnerOuterBeliefImaginationLibrary::DefinitionOfImagination(EInnerOuterImagination V)
{
    switch (V)
    {
    case EInnerOuterImagination::InnerImagination:
        return TEXT("Is The Imagination Forming Within The Inner Spaces Made Within The Brain And/Or Mind");
    case EInnerOuterImagination::OuterImagination:
        return TEXT("Is The Imagination Forming Visual Responses Within The World Itself Using Vision Instead Of Sight");
    }
    return FString();
}

