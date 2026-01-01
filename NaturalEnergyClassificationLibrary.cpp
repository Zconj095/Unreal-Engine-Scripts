#include "NaturalEnergyClassificationLibrary.h"

FString UNaturalEnergyClassificationLibrary::ToLabel_NaturalParticleUnderstanding(ENaturalParticleUnderstanding V)
{
    switch (V)
    {
    case ENaturalParticleUnderstanding::NaturalEnergies:          return TEXT("NATURAL ENERGIES");
    default:                                                      return TEXT("NATURALLY DEFINED ENERGIES");
    }
}

FString UNaturalEnergyClassificationLibrary::ToLabel_EnergyRelated(EEnergyRelated V)
{
    switch (V)
    {
    case EEnergyRelated::EnergyClassification:       return TEXT("ENERGY CLASSIFICATION");
    case EEnergyRelated::EnergyGenreClassification:  return TEXT("ENERGY GENRE CLASSIFICATION");
    default:                                         return TEXT("ENERGY GROUPING");
    }
}

FString UNaturalEnergyClassificationLibrary::ToLabel_EnergyClass(EEnergyClass V)
{
    switch (V)
    {
    case EEnergyClass::ArtificialEnergy: return TEXT("ARTIFICIAL ENERGY");
    default:                             return TEXT("NATURAL ENERGY");
    }
}

FString UNaturalEnergyClassificationLibrary::ToLabel_EnergyType(EEnergyType V)
{
    switch (V)
    {
    case EEnergyType::Willed:    return TEXT("WILLED ENERGY");
    case EEnergyType::Pressured: return TEXT("PRESSURED ENERGY");
    default:                     return TEXT("FORCED ENERGY");
    }
}

FString UNaturalEnergyClassificationLibrary::ToLabel_VocalClass(EVocalClass V)
{
    switch (V)
    {
    case EVocalClass::ArtificialVocals: return TEXT("ARTIFICIAL VOCALS");
    default:                            return TEXT("NATURAL VOCALS");
    }
}

FString UNaturalEnergyClassificationLibrary::ToLabel_VocalType(EVocalType V)
{
    switch (V)
    {
    case EVocalType::Willed:    return TEXT("WILLED VOCALS");
    case EVocalType::Pressured: return TEXT("PRESSURED VOCALS");
    default:                    return TEXT("FORCED VOCALS");
    }
}

FString UNaturalEnergyClassificationLibrary::ToLabel_BreathingPatternClass(EBreathingPatternClass V)
{
    switch (V)
    {
    case EBreathingPatternClass::ArtificialBreathingPatterns: return TEXT("ARTIFICIAL BREATHING PATTERNS");
    default:                                                  return TEXT("NATURAL BREATHING PATTERNS");
    }
}

FString UNaturalEnergyClassificationLibrary::ToLabel_ChemistryFocus(EChemistryFocusType V)
{
    switch (V)
    {
    case EChemistryFocusType::Biochemistry:   return TEXT("BIOCHEMISTRY");
    case EChemistryFocusType::Neurochemistry: return TEXT("NEUROCHEMISTRY");
    default:                                  return TEXT("GEOCHEMISTRY");
    }
}

