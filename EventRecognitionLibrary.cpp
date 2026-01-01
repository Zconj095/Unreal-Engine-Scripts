#include "EventRecognitionLibrary.h"

FString UEventRecognitionLibrary::ToLabel_Category(EEventCategory Cat)
{
    switch (Cat)
    {
    case EEventCategory::SpecialEvent:      return TEXT("Special Events");
    default:                                return TEXT("Special Occurrences");
    }
}

FString UEventRecognitionLibrary::ToLabel_WillType(EEventWillType Will)
{
    switch (Will)
    {
    case EEventWillType::Willed:    return TEXT("Willed Events");
    case EEventWillType::Pressured: return TEXT("Pressured Events");
    default:                        return TEXT("Forced Events");
    }
}

static bool ContainsAny(const FString& S, const TArray<FString>& Keys)
{
    const FString L = S.ToLower();
    for (const FString& K : Keys)
    {
        if (L.Contains(K)) return true;
    }
    return false;
}

FEventRecognitionReport UEventRecognitionLibrary::RecognizeEventText(const FString& TitleOrDescription)
{
    FEventRecognitionReport R; R.bRecognized = false; R.Confidence = 0.f;
    const FString T = TitleOrDescription;

    // Category heuristics
    bool bEvent = ContainsAny(T, { TEXT("event"), TEXT("festival"), TEXT("ceremony"), TEXT("anniversary"), TEXT("birthday"), TEXT("graduation"), TEXT("wedding") });
    bool bOccurrence = ContainsAny(T, { TEXT("occurrence"), TEXT("incident"), TEXT("phenomenon"), TEXT("anomaly"), TEXT("rare"), TEXT("unexpected") });
    if (bEvent && !bOccurrence) { R.Category = EEventCategory::SpecialEvent; R.Confidence += 0.5f; R.Rationale += TEXT("Category=Event; "); }
    else if (bOccurrence && !bEvent) { R.Category = EEventCategory::SpecialOccurrence; R.Confidence += 0.5f; R.Rationale += TEXT("Category=Occurrence; "); }
    else if (bEvent && bOccurrence) { R.Category = EEventCategory::SpecialOccurrence; R.Confidence += 0.3f; R.Rationale += TEXT("Mixed keywords; leaning Occurrence; "); }
    else { R.Category = EEventCategory::SpecialEvent; R.Rationale += TEXT("Default category=Event; "); }

    // Will type heuristics
    if (ContainsAny(T, { TEXT("voluntary"), TEXT("chose"), TEXT("chosen"), TEXT("decided"), TEXT("willing"), TEXT("intentional") }))
    {
        R.WillType = EEventWillType::Willed; R.Confidence += 0.5f; R.Rationale += TEXT("Willed; ");
    }
    else if (ContainsAny(T, { TEXT("pressure"), TEXT("pressured"), TEXT("coerced"), TEXT("forced"), TEXT("obliged"), TEXT("compelled") }))
    {
        // Distinguish pressured vs forced
        if (ContainsAny(T, { TEXT("forced"), TEXT("coerced"), TEXT("compelled") }))
        { R.WillType = EEventWillType::Forced; R.Confidence += 0.5f; R.Rationale += TEXT("Forced; "); }
        else
        { R.WillType = EEventWillType::Pressured; R.Confidence += 0.45f; R.Rationale += TEXT("Pressured; "); }
    }
    else
    {
        R.WillType = EEventWillType::Willed; R.Rationale += TEXT("Default will=Willed; ");
    }

    R.bRecognized = (R.Confidence >= 0.4f);
    R.Confidence = FMath::Clamp(R.Confidence, 0.f, 1.f);
    return R;
}

FEventRecognitionReport UEventRecognitionLibrary::ComposeEventRecognition(EEventCategory Category, EEventWillType WillType, float Confidence, const FString& Rationale)
{
    FEventRecognitionReport R; R.bRecognized = true; R.Category = Category; R.WillType = WillType; R.Confidence = FMath::Clamp(Confidence, 0.f, 1.f); R.Rationale = Rationale;
    return R;
}

