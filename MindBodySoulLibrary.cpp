#include "MindBodySoulLibrary.h"

// Labels
FString UMindBodySoulLibrary::ToLabel_Construct(ECoreConstruct V)
{
    switch (V)
    {
    case ECoreConstruct::Mind:   return TEXT("MIND");
    case ECoreConstruct::Body:   return TEXT("BODY");
    case ECoreConstruct::Soul:   return TEXT("SOUL");
    case ECoreConstruct::Spirit: return TEXT("SPIRIT");
    default:                     return TEXT("BRAIN");
    }
}

FString UMindBodySoulLibrary::ToLabel_State(EFunctionalState V)
{
    switch (V)
    {
    case EFunctionalState::Mental:    return TEXT("MENTAL STATE");
    case EFunctionalState::Emotional: return TEXT("EMOTIONAL STATE");
    case EFunctionalState::Physical:  return TEXT("PHYSICAL STATE");
    default:                          return TEXT("SPIRITUAL STATE");
    }
}

FString UMindBodySoulLibrary::ToLabel_BeliefType(EBeliefType V)
{
    switch (V)
    {
    case EBeliefType::Belief:      return TEXT("BELIEF");
    case EBeliefType::Virtues:     return TEXT("VIRTUES");
    case EBeliefType::Morals:      return TEXT("MORALS");
    case EBeliefType::Values:      return TEXT("VALUES");
    case EBeliefType::Truths:      return TEXT("TRUTHS");
    default:                       return TEXT("DISCIPLINES");
    }
}

// Definitions
FString UMindBodySoulLibrary::DefinitionOf_Construct(ECoreConstruct V)
{
    switch (V)
    {
    case ECoreConstruct::Mind:
        return TEXT("The space within the brain that connects the brain and consciousness together and connects imagination and creativity together.");
    case ECoreConstruct::Body:
        return TEXT("The physical connection that holds the mind, soul, spirit and brain.");
    case ECoreConstruct::Soul:
        return TEXT("The connection to one's values, cores, beliefs, memories, existence and emotions, moods and feelings.");
    case ECoreConstruct::Spirit:
        return TEXT("The energy that connects with the body and derives from the capability of existence and willpower to exist within the body and spirit.");
    case ECoreConstruct::Brain:
        return TEXT("The computational system that connects the mind, body, soul and spirit together.");
    }
    return FString();
}

FString UMindBodySoulLibrary::DefinitionOf_State(EFunctionalState V)
{
    switch (V)
    {
    case EFunctionalState::Mental:
        return TEXT("The state that handles all mental connections between the brain and/or mind.");
    case EFunctionalState::Emotional:
        return TEXT("The state that handles all emotional connections between the brain, mind and body; influenced by the spiritual state; controls, recognizes, manages and contains all recognized emotions, moods and feelings connected to the body and/or mind.");
    case EFunctionalState::Physical:
        return TEXT("The state connecting to the body and the relations of the body to the mental, emotional and spiritual states.");
    case EFunctionalState::Spiritual:
        return TEXT("The state that connects spiritual existence and beliefs to the mind, soul and spirit through the mental and emotional states, and how it connects to beliefs formed from mind and/or soul and/or spirit.");
    }
    return FString();
}

FString UMindBodySoulLibrary::DefinitionOf_BeliefType(EBeliefType V)
{
    switch (V)
    {
    case EBeliefType::Belief:
        return TEXT("Something formed from an idea and/or series of ideas.");
    case EBeliefType::Virtues:
        return TEXT("A belief inherited from another connection (or from instructions) and/or inherited as a virtue.");
    case EBeliefType::Morals:
        return TEXT("A specific list of rules and/or boundaries that are continuously improved and strengthened within time and space.");
    case EBeliefType::Values:
        return TEXT("Boundaries and/or parameters that are followed with a specific list of truths and/or beliefs and/or belief systems.");
    case EBeliefType::Truths:
        return TEXT("A list of beliefs formed as internal systems of interconnected belief systems that interconnect through experience and/or wisdom and/or disciplines and/or practice.");
    case EBeliefType::Disciplines:
        return TEXT("A specific list of rules and/or guidelines that are repeatedly followed and formed through repeated practice and/or usage until connections strengthen and affect actions and behaviors.");
    }
    return FString();
}

// Lists
TArray<ECoreConstruct> UMindBodySoulLibrary::AllConstructs()
{
    return { ECoreConstruct::Mind, ECoreConstruct::Body, ECoreConstruct::Soul, ECoreConstruct::Spirit, ECoreConstruct::Brain };
}

TArray<EFunctionalState> UMindBodySoulLibrary::AllStates()
{
    return { EFunctionalState::Mental, EFunctionalState::Emotional, EFunctionalState::Physical, EFunctionalState::Spiritual };
}

TArray<EBeliefType> UMindBodySoulLibrary::AllBeliefTypes()
{
    return { EBeliefType::Belief, EBeliefType::Virtues, EBeliefType::Morals, EBeliefType::Values, EBeliefType::Truths, EBeliefType::Disciplines };
}

