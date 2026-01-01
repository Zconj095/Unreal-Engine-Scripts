#include "ContentClassificationLibrary.h"

// Labels
FString UContentClassificationLibrary::ToLabel_AdultMaturity(EAdultAgeSpecificMaturity V)
{
    switch (V)
    {
    case EAdultAgeSpecificMaturity::SectionOne: return TEXT("IS CONTENT IMMATURE FOR AGE");
    default:                                   return TEXT("IS CONTENT MATURE FOR AGE");
    }
}

FString UContentClassificationLibrary::ToLabel_AgeCompliment(EMatureAdultWithAgeCompliment V)
{
    return TEXT("EIGHTEEN PLUS");
}

FString UContentClassificationLibrary::ToLabel_VideoRating(EVideoRating V)
{
    switch (V)
    {
    case EVideoRating::RatingOne:   return TEXT("AGE SPECIFIC CONTENT");
    case EVideoRating::RatingTwo:   return TEXT("GENERAL AUDIENCES");
    case EVideoRating::RatingThree: return TEXT("MATURE AUDIENCES ONLY");
    case EVideoRating::RatingFour:  return TEXT("PARENTAL GUIDANCE");
    case EVideoRating::RatingFive:  return TEXT("PARENTAL GUIDANCE UNDER THIRTEEN");
    default:                        return TEXT("RESTRICTED AUDIENCES");
    }
}

FString UContentClassificationLibrary::ToLabel_LimiterRange(ELimiterRange V)
{
    switch (V)
    {
    case ELimiterRange::RangeOne:   return TEXT("PRIVATE ACCESS");
    case ELimiterRange::RangeTwo:   return TEXT("PERSONAL ACCESS");
    case ELimiterRange::RangeThree: return TEXT("LOCAL ACCESS");
    case ELimiterRange::RangeFour:  return TEXT("HOME ACCESS");
    default:                        return TEXT("PUBLIC ACCESS");
    }
}

FString UContentClassificationLibrary::ToLabel_DomainClass(EDomainClassCollection V)
{
    switch (V)
    {
    case EDomainClassCollection::ClassOne:  return TEXT("DOMAIN CLASSES");
    case EDomainClassCollection::ClassTwo:  return TEXT("DOMAIN SUBTYPES");
    default:                                return TEXT("DOMAIN TYPES");
    }
}

FString UContentClassificationLibrary::ToLabel_DomainType(EDomainTypeClass V)
{
    switch (V)
    {
    case EDomainTypeClass::Local:                       return TEXT("LOCAL");
    case EDomainTypeClass::LocalPersonalDomains:        return TEXT("LOCAL PERSONAL DOMAINS");
    case EDomainTypeClass::LocalPersonalPrivateDomains: return TEXT("LOCAL PERSONAL PRIVATE DOMAINS");
    case EDomainTypeClass::LocalPrivateDomains:         return TEXT("LOCAL PRIVATE DOMAINS");
    case EDomainTypeClass::LocalPrivatePersonalDomains: return TEXT("LOCAL PRIVATE PERSONAL DOMAINS");
    case EDomainTypeClass::LocalPrivatePublicDomains:   return TEXT("LOCAL PRIVATE PUBLIC DOMAINS");
    case EDomainTypeClass::Private:                     return TEXT("PRIVATE");
    case EDomainTypeClass::PrivateUsageOnly:            return TEXT("PRIVATE USAGE ONLY");
    default:                                            return TEXT("PUBLIC USAGE ONLY");
    }
}

FString UContentClassificationLibrary::ToLabel_Virtue(EHeavenlyVirtue V)
{
    switch (V)
    {
    case EHeavenlyVirtue::Patience:   return TEXT("PATIENCE");
    case EHeavenlyVirtue::Chastity:   return TEXT("CHASTITY");
    case EHeavenlyVirtue::Temperance: return TEXT("TEMPERANCE");
    case EHeavenlyVirtue::Clarity:    return TEXT("CLARITY");
    case EHeavenlyVirtue::Grace:      return TEXT("GRACE");
    case EHeavenlyVirtue::Calmness:   return TEXT("CALMNESS");
    case EHeavenlyVirtue::Diligence:  return TEXT("DILIGENCE");
    case EHeavenlyVirtue::Serene:     return TEXT("SERENE");
    default:                          return TEXT("KINDNESS");
    }
}

// Age checks
bool UContentClassificationLibrary::MeetsAdultRequirement(int32 UserAge, EMatureAdultWithAgeCompliment Requirement)
{
    // Only defined subclass is 18+
    return UserAge >= 18;
}

EVideoRating UContentClassificationLibrary::SuggestedMinimumRatingForAge(int32 UserAge)
{
    if (UserAge < 13)  return EVideoRating::RatingFive;   // PG under 13
    if (UserAge < 17)  return EVideoRating::RatingFour;   // PG
    if (UserAge < 18)  return EVideoRating::RatingSix;    // Restricted (guardian)
    return EVideoRating::RatingTwo;                        // General or above
}

FComplianceReport UContentClassificationLibrary::IsContentCompliantForUser(int32 UserAge, const FContentPolicy& Policy)
{
    FComplianceReport Report;

    // Age requirement for adult compliment and mature-only ratings
    const bool bAdultOK = MeetsAdultRequirement(UserAge, Policy.AgeClass);
    if (!bAdultOK)
    {
        Report.Warnings.Add(TEXT("User must be 18+ for this content."));
    }

    // Enforce ratings
    switch (Policy.Rating)
    {
    case EVideoRating::RatingThree: // MATURE AUDIENCES ONLY
    case EVideoRating::RatingSix:   // RESTRICTED AUDIENCES
        if (UserAge < 18)
        {
            Report.Warnings.Add(TEXT("Mature/Restricted rating requires 18+."));
        }
        break;
    default:
        break;
    }

    // Limiter ranges are informational here; you can add environment checks if needed
    // Domain classes/types are descriptive; no hard restriction in this helper

    Report.bCompliant = (Report.Warnings.Num() == 0);
    return Report;
}

