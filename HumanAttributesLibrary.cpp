#include "HumanAttributesLibrary.h"

FString UHumanAttributesLibrary::ToLabel_Attribute(EHumanAttribute Attr)
{
    switch (Attr)
    {
    case EHumanAttribute::HumanAura:              return TEXT("HUMAN AURA");
    case EHumanAttribute::HumanChakras:           return TEXT("HUMAN CHAKRAS");
    case EHumanAttribute::HumanEMField:           return TEXT("HUMAN EM FIELD");
    case EHumanAttribute::HumanBiology:           return TEXT("HUMAN BIOLOGY");
    case EHumanAttribute::HumanAnatomy:           return TEXT("HUMAN ANATOMY");
    case EHumanAttribute::HumanBioenergeticField: return TEXT("HUMAN BIOENERGETIC FIELD");
    case EHumanAttribute::HumanBodyResonance:     return TEXT("HUMAN BODY RESONANCE");
    default:                                      return TEXT("HUMAN BRAINWAVE ANATOMY");
    }
}

FString UHumanAttributesLibrary::ToLabel_Keyword(EConceptKeyword Key)
{
    switch (Key)
    {
    case EConceptKeyword::Beliefs:      return TEXT("BELIEFS");
    case EConceptKeyword::Instructions: return TEXT("INSTRUCTIONS");
    case EConceptKeyword::Intent:       return TEXT("INTENT");
    case EConceptKeyword::Will:         return TEXT("WILL");
    case EConceptKeyword::Energy:       return TEXT("ENERGY");
    default:                            return TEXT("ENERGIES");
    }
}

FString UHumanAttributesLibrary::DefinitionOf_Attribute(EHumanAttribute Attr)
{
    switch (Attr)
    {
    case EHumanAttribute::HumanAura:
        return TEXT("A conceptual subtle field surrounding the body, often associated with personal energy, mood, and well‑being.");
    case EHumanAttribute::HumanChakras:
        return TEXT("A framework of energy centers aligned through the body used to describe flows, balance, and focus of personal energy.");
    case EHumanAttribute::HumanEMField:
        return TEXT("The body's electromagnetic field arising from bioelectric activity (e.g., heart and brain), measurable with instruments.");
    case EHumanAttribute::HumanBiology:
        return TEXT("The science of life processes in humans, including cells, tissues, organs, metabolism, and homeostasis.");
    case EHumanAttribute::HumanAnatomy:
        return TEXT("The structure of the human body, describing systems, organs, and spatial relationships of tissues.");
    case EHumanAttribute::HumanBioenergeticField:
        return TEXT("A conceptual field describing interactions of biological function and energy transformation in and around the body.");
    case EHumanAttribute::HumanBodyResonance:
        return TEXT("Natural frequencies and resonant behaviors of body tissues and systems in response to mechanical or acoustic excitation.");
    case EHumanAttribute::HumanBrainwaveAnatomy:
        return TEXT("Organization of brainwave patterns (delta, theta, alpha, beta, gamma) and their regional/functional associations.");
    }
    return FString();
}

FString UHumanAttributesLibrary::DefinitionOf_Keyword(EConceptKeyword Key)
{
    switch (Key)
    {
    case EConceptKeyword::Beliefs:
        return TEXT("Internal models and convictions that shape interpretation, motivation, and decision‑making.");
    case EConceptKeyword::Instructions:
        return TEXT("Guiding procedures or steps used to direct actions, learning, or system behavior.");
    case EConceptKeyword::Intent:
        return TEXT("Purposeful orientation of action toward an outcome; the 'why' behind behavior.");
    case EConceptKeyword::Will:
        return TEXT("Volitional capacity to choose, commit, and sustain action despite obstacles.");
    case EConceptKeyword::Energy:
        return TEXT("Capacity to do work or produce change; in humans spanning metabolic, mechanical, electrical, and conceptual domains.");
    case EConceptKeyword::Energies:
        return TEXT("Plural forms or modalities of energy (e.g., chemical, electrical, emotional, spiritual) considered collectively.");
    }
    return FString();
}

TArray<EHumanAttribute> UHumanAttributesLibrary::AllAttributes()
{
    return {
        EHumanAttribute::HumanAura,
        EHumanAttribute::HumanChakras,
        EHumanAttribute::HumanEMField,
        EHumanAttribute::HumanBiology,
        EHumanAttribute::HumanAnatomy,
        EHumanAttribute::HumanBioenergeticField,
        EHumanAttribute::HumanBodyResonance,
        EHumanAttribute::HumanBrainwaveAnatomy
    };
}

TArray<EConceptKeyword> UHumanAttributesLibrary::AllKeywords()
{
    return {
        EConceptKeyword::Beliefs,
        EConceptKeyword::Instructions,
        EConceptKeyword::Intent,
        EConceptKeyword::Will,
        EConceptKeyword::Energy,
        EConceptKeyword::Energies
    };
}

