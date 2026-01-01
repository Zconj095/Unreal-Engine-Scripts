#include "RecognitionFormsLibrary.h"

// Labels (straight mappings to DisplayName strings)
FString URecognitionFormsLibrary::ToLabel_HandlingMethod(EHandlingMethod V)
{
    switch (V)
    {
    case EHandlingMethod::Control:    return TEXT("CONTROL");
    case EHandlingMethod::Use:        return TEXT("USE");
    case EHandlingMethod::Manage:     return TEXT("MANAGE");
    case EHandlingMethod::Manipulate: return TEXT("MANIPULATE");
    default:                          return TEXT("GUIDE");
    }
}

FString URecognitionFormsLibrary::ToLabel_HandlingDomain(EHandlingDomain V)
{
    switch (V)
    {
    case EHandlingDomain::Biochemistry:   return TEXT("BIOCHEMISTRY");
    case EHandlingDomain::Neurochemistry: return TEXT("NEUROCHEMISTRY");
    case EHandlingDomain::Geochemistry:   return TEXT("GEOCHEMISTRY");
    case EHandlingDomain::Energies:       return TEXT("ENERGIES");
    case EHandlingDomain::Energy:         return TEXT("ENERGY");
    case EHandlingDomain::Particles:      return TEXT("PARTICLES");
    case EHandlingDomain::Ions:           return TEXT("IONS");
    case EHandlingDomain::Molecules:      return TEXT("MOLECULES");
    case EHandlingDomain::Atoms:          return TEXT("ATOMS");
    case EHandlingDomain::Thoughts:       return TEXT("THOUGHTS");
    case EHandlingDomain::Beliefs:        return TEXT("BELIEFS");
    case EHandlingDomain::Voice:          return TEXT("THE VOICE");
    default:                              return TEXT("VOCALS AND OR SPEECH");
    }
}

FString URecognitionFormsLibrary::ToLabel_FormedBy(EFormedBy V)
{
    switch (V)
    {
    case EFormedBy::Will:     return TEXT("FORMED BY WILL");
    case EFormedBy::Pressure: return TEXT("FORMED BY PRESSURE");
    default:                  return TEXT("FORMED BY FORCE");
    }
}

FString URecognitionFormsLibrary::ToLabel_ChemistryFormingRelationship(EChemistryFormingRelationship V)
{
    switch (V)
    {
    case EChemistryFormingRelationship::DependentToIndependent:   return TEXT("DEPENDENT TO INDEPENDENT");
    case EChemistryFormingRelationship::DependentToCodependent:   return TEXT("DEPENDENT TO CODEPENDENT");
    case EChemistryFormingRelationship::IndependentToDependent:   return TEXT("INDEPENDENT TO DEPENDENT");
    default:                                                      return TEXT("INDEPENDENT TO CODEPENDENT");
    }
}

FString URecognitionFormsLibrary::ToLabel_ChemistryFormFactor(EChemistryFormFactor V)
{
    switch (V)
    {
    case EChemistryFormFactor::Instructions:  return TEXT("INSTRUCTIONS");
    case EChemistryFormFactor::Intent:        return TEXT("INTENT");
    case EChemistryFormFactor::Beliefs:       return TEXT("BELIEFS");
    default:                                  return TEXT("COMPATIBILITY");
    }
}

FString URecognitionFormsLibrary::ToLabel_WayOnTop(EWayOnTop V)
{
    switch (V)
    {
    case EWayOnTop::Priority: return TEXT("PRIORITY");
    case EWayOnTop::Primary:  return TEXT("PRIMARY");
    default:                  return TEXT("MASTER");
    }
}

FString URecognitionFormsLibrary::ToLabel_UnionForm(EUnionForm V)
{
    switch (V)
    {
    case EUnionForm::Marriage:      return TEXT("MARRIAGE");
    default:                        return TEXT("HOLY MATRIMONY");
    }
}

FString URecognitionFormsLibrary::ToLabel_AgreementForm(EAgreementForm V)
{
    switch (V)
    {
    case EAgreementForm::Consensual: return TEXT("CONSENSUAL");
    case EAgreementForm::Forced:     return TEXT("FORCED");
    case EAgreementForm::Arranged:   return TEXT("ARRANGED");
    default:                         return TEXT("PRESSURED");
    }
}

FString URecognitionFormsLibrary::ToLabel_WayOfBeingOwned(EWayOfBeingOwned V)
{
    switch (V)
    {
    case EWayOfBeingOwned::ByInstructionsFollowed:              return TEXT("BY INSTRUCTIONS THAT ONE FOLLOWS");
    case EWayOfBeingOwned::ByMechanicalEnergyUsed:              return TEXT("BY MECHANICAL ENERGY THAT ONE USES");
    case EWayOfBeingOwned::ByChemistrySuppliedOrGiven:          return TEXT("BY CHEMISTRY AND WHATS BEEN SUPPLIED AND OR GIVEN");
    case EWayOfBeingOwned::ByEnergyAndHowItForms:               return TEXT("BY ENERGY ITSELF AND HOW IT FORMS");
    case EWayOfBeingOwned::ByBeliefsAndHowFormed:               return TEXT("BY BELIEFS AND HOW THEY ARE FORMED/MADE/CREATED");
    case EWayOfBeingOwned::ByWillAndConsent:                    return TEXT("BY WILL AND HOW IT FORMS BY CONSENT");
    case EWayOfBeingOwned::ByIntentionsWithOrWithoutInstructions:return TEXT("BY INTENTIONS AND HOW THEY ARE CREATED WITH AND OR WITHOUT INSTRUCTIONS");
    case EWayOfBeingOwned::ByRoadsPathsWalkedDriven:            return TEXT("BY ROADS/PATHS THAT ARE WALKED AND OR DRIVEN");
    case EWayOfBeingOwned::ByMachinesAndTheirTreatment:         return TEXT("BY MACHINES AND HOW THEY ARE TREATED AND OR THE INSTRUCTIONS AND OR INTENTIONS THAT BIND THEM");
    case EWayOfBeingOwned::ByHabitsBehaviors:                   return TEXT("BY HABITS AND OR BEHAVIORS AND HOW THEY ARE UNDERSTOOD AND OR FORMED");
    case EWayOfBeingOwned::ByDendritesFormBreakdown:            return TEXT("BY DENDRITES AND HOW THEY ARE FORMED AND HOW THEY ARE BROKEN DOWN");
    default:                                                    return TEXT("BY VOCALS AND HOW THEY FORM");
    }
}

FString URecognitionFormsLibrary::ToLabel_FormedChemistryClass(EFormedChemistryClass V)
{
    switch (V)
    {
    case EFormedChemistryClass::NaturallyFormed:    return TEXT("NATURALLY FORMED");
    case EFormedChemistryClass::ArtificiallyFormed: return TEXT("ARTIFICIALLY FORMED");
    case EFormedChemistryClass::SyntheticallyFormed:return TEXT("SYNTHETICALLY FORMED");
    case EFormedChemistryClass::HybridFormed:       return TEXT("FORMED AS A HYBRID");
    default:                                        return TEXT("ORIGINAL");
    }
}

FString URecognitionFormsLibrary::ToLabel_BeliefClass(EBeliefClass V)
{
    switch (V)
    {
    case EBeliefClass::NaturallyFormed:     return TEXT("NATURALLY FORMED");
    case EBeliefClass::ArtificiallyFormed:  return TEXT("ARTIFICIALLY FORMED");
    case EBeliefClass::SyntheticallyFormed: return TEXT("SYNTHETICALLY FORMED");
    case EBeliefClass::HybridFormed:        return TEXT("FORMED AS A HYBRID");
    case EBeliefClass::FormedByInheritance: return TEXT("FORMED BY INHERITANCE");
    case EBeliefClass::Original:            return TEXT("ORIGINAL");
    case EBeliefClass::FormedByWill:        return TEXT("FORMED BY WILL");
    case EBeliefClass::FormedByPressure:    return TEXT("FORMED BY PRESSURE");
    default:                                return TEXT("FORMED BY FORCE");
    }
}

FString URecognitionFormsLibrary::ToLabel_ThoughtClass(EThoughtClass V)
{
    switch (V)
    {
    case EThoughtClass::NaturallyFormed:     return TEXT("NATURALLY FORMED");
    case EThoughtClass::ArtificiallyFormed:  return TEXT("ARTIFICIALLY FORMED");
    case EThoughtClass::SyntheticallyFormed: return TEXT("SYNTHETICALLY FORMED");
    case EThoughtClass::HybridFormed:        return TEXT("FORMED AS A HYBRID");
    case EThoughtClass::FormedByInheritance: return TEXT("FORMED BY INHERITANCE");
    case EThoughtClass::Original:            return TEXT("ORIGINAL");
    case EThoughtClass::FormedByWill:        return TEXT("FORMED BY WILL");
    case EThoughtClass::FormedByPressure:    return TEXT("FORMED BY PRESSURE");
    default:                                 return TEXT("FORMED BY FORCE");
    }
}

FString URecognitionFormsLibrary::ComposeHandlingLabel(const FHandlingDirective& D)
{
    return ToLabel_HandlingMethod(D.Method) + TEXT(" ") + ToLabel_HandlingDomain(D.Domain);
}

