#include "AffectMetricsLibrary.h"

FString UAffectMetricsLibrary::ToLabel_Domain(EAffectDomain Domain)
{
    switch (Domain)
    {
    case EAffectDomain::Mood:    return TEXT("Mood");
    case EAffectDomain::Feeling: return TEXT("Feeling");
    default:                     return TEXT("Emotion");
    }
}

FString UAffectMetricsLibrary::ToLabel_Measure(EAffectMeasureKind Kind)
{
    switch (Kind)
    {
    case EAffectMeasureKind::Throughput:    return TEXT("Throughput");
    case EAffectMeasureKind::Magnitude:     return TEXT("Magnitude");
    case EAffectMeasureKind::Understanding: return TEXT("Understanding");
    case EAffectMeasureKind::Clarity:       return TEXT("Clarity");
    case EAffectMeasureKind::Enlightenment: return TEXT("Enlightenment");
    case EAffectMeasureKind::Presence:      return TEXT("Presence");
    default:                                return TEXT("Essence");
    }
}

static FString CombinedLabel(EAffectDomain Domain)
{
    switch (Domain)
    {
    case EAffectDomain::Mood:    return TEXT("Mood Based Throughput And Mood Based Magnitude");
    case EAffectDomain::Feeling: return TEXT("Feeling Based Throughput And Feeling Based Magnitude");
    default:                     return TEXT("Emotional Throughput And Emotional Magnitude");
    }
}

FString UAffectMetricsLibrary::DefinitionOf_Combined(EAffectDomain Domain)
{
    switch (Domain)
    {
    case EAffectDomain::Mood:
        return TEXT("Are Two Ideas That Are Used To Describe The Way That People Experience And Express Their Moods");
    case EAffectDomain::Feeling:
        return TEXT("Are Two Ideas That Are Used To Describe The Way That People Experience And Express Their Feelings");
    case EAffectDomain::Emotion:
        return TEXT("Are Two Concepts That Are Used To Describe The Way That People Experience And Express Emotions");
    }
    return FString();
}

FString UAffectMetricsLibrary::DefinitionOf_Measure(EAffectDomain Domain, EAffectMeasureKind Kind)
{
    if (Kind == EAffectMeasureKind::Throughput)
    {
        switch (Domain)
        {
        case EAffectDomain::Mood:    return TEXT("Refers To The Rate At Which People Experience Moods");
        case EAffectDomain::Feeling: return TEXT("Refers To The Rate At Which People Experience Feelings");
        case EAffectDomain::Emotion: return TEXT("Refers To The Rate At Which People Experience Emotions");
        }
    }
    if (Kind == EAffectMeasureKind::Magnitude)
    {
        switch (Domain)
        {
        case EAffectDomain::Mood:    return TEXT("Refers To The Intensity Of The Moods That Someone Or People Can Experience");
        case EAffectDomain::Feeling: return TEXT("Refers To The Intensity Of The Feelings That Someone Or People Can Experience");
        case EAffectDomain::Emotion: return TEXT("Refers To The Intensity Of The Emotions That People Experience");
        }
    }
    if (Kind == EAffectMeasureKind::Understanding)
    {
        switch (Domain)
        {
        case EAffectDomain::Mood:
            return TEXT("Is The Ability To Recognize And Understand The Individual Moods Of A Person As Well As The Feelings Of Others And It Is A Complex Process That Includes The Thoughts Emotions Feelings And Behaviors Of A Person");
        case EAffectDomain::Feeling:
            return TEXT("Is The Ability To Recognize And Understand The Individual Feelings Of A Person As Well As The Feelings Of Others And It Is A Complex Process That Includes The Thoughts Emotions Moods And Behaviors Of A Person");
        case EAffectDomain::Emotion:
            return TEXT("Is The Ability To Recognize And Understand The Individual Emotions Of A Person As Well As The Emotions Of Others And It Is A Complex Process That Includes The Thoughts Feelings And Behaviors Of A Person");
        }
    }
    if (Kind == EAffectMeasureKind::Clarity)
    {
        switch (Domain)
        {
        case EAffectDomain::Mood:    return TEXT("Is The Ability To Recognize Locate Understand And Express Moods With Accurate Measurements");
        case EAffectDomain::Feeling: return TEXT("Is The Ability To Recognize Locate Understand And Express Feelings With Accurate Measurements");
        case EAffectDomain::Emotion: return TEXT("Is The Ability To Recognize Locate Understand And Express Emotions With Accurate Measurements");
        }
    }
    if (Kind == EAffectMeasureKind::Enlightenment)
    {
        switch (Domain)
        {
        case EAffectDomain::Mood:    return TEXT("Is A State Of Being In Which The Entity Is Fully Aware Of Their Moods And How They Affect The Entity");
        case EAffectDomain::Feeling: return TEXT("Is A State Of Being In Which The Entity Is Fully Aware Of Their Feelings And How They Affect The Entity");
        case EAffectDomain::Emotion: return TEXT("Is A State Of Being In Which The Entity Is Fully Aware Of The Emotions And How They Affect The Entity");
        }
    }
    if (Kind == EAffectMeasureKind::Presence)
    {
        return TEXT("The ability to be fully present in the moment and to be aware of your own and others' emotions. It supports relationships and positive, productive environments.");
    }
    if (Kind == EAffectMeasureKind::Essence)
    {
        return TEXT("The core of who we are as individuals—the sum of our emotions, both positive and negative—enabling deep connection and authentic living.");
    }
    return FString();
}

FString UAffectMetricsLibrary::Definition_InnerOuterEmotions()
{
    return TEXT("Inner emotions are the feelings experienced internally; outer emotions are their expressions via face, body, tone, and words. Inner emotions drive outer expressions, outer expressions can influence inner emotions, and thoughts can shape both.");
}

FString UAffectMetricsLibrary::Definition_EmotionalEssence()
{
    return TEXT("Emotional essence is the core of who we are—the totality of our emotions. Becoming aware of and expressing this essence supports authenticity and deeper connection.");
}

FString UAffectMetricsLibrary::Definition_EmotionalPresence()
{
    return TEXT("Emotional presence is being fully in the moment with awareness of your own and others' emotions. It fosters trust, safety, and effective collaboration.");
}

TArray<FString> UAffectMetricsLibrary::Advice_ForEmpaths()
{
    return {
        TEXT("Set boundaries: say no when capacity is limited; protect time and space to recharge."),
        TEXT("Practice physical self‑care: nutrition, sleep, and regular movement."),
        TEXT("Schedule restorative activities you genuinely enjoy to de‑stress."),
        TEXT("Balance compassion with self‑protection to reduce overwhelm and burnout.")
    };
}

FAffectMetrics UAffectMetricsLibrary::ComputeMetrics(const FAffectMetricsInput& Input)
{
    FAffectMetrics Out;
    Out.ThroughputPerMinute = ComputeThroughputPerMinute(Input.EventCount, Input.DurationSeconds);
    const FAffectMetrics Mag = ComputeMagnitudeStats(Input.Intensities);
    Out.AverageMagnitude = Mag.AverageMagnitude;
    Out.PeakMagnitude = Mag.PeakMagnitude;
    Out.MagnitudeStdDev = Mag.MagnitudeStdDev;
    Out.Summary = FString::Printf(TEXT("TPM=%.2f, Avg=%.3f, Peak=%.3f, Std=%.3f"), Out.ThroughputPerMinute, Out.AverageMagnitude, Out.PeakMagnitude, Out.MagnitudeStdDev);
    return Out;
}

double UAffectMetricsLibrary::ComputeThroughputPerMinute(int32 EventCount, double DurationSeconds)
{
    if (DurationSeconds <= 0.0) return 0.0;
    return 60.0 * double(EventCount) / DurationSeconds;
}

FAffectMetrics UAffectMetricsLibrary::ComputeMagnitudeStats(const TArray<double>& Intensities)
{
    FAffectMetrics Out;
    if (Intensities.Num() == 0) return Out;

    double sum = 0.0, sumsq = 0.0; double mn = Intensities[0], mx = Intensities[0];
    for (double v : Intensities) { sum += v; sumsq += v*v; mn = FMath::Min(mn, v); mx = FMath::Max(mx, v); }
    const double n = double(Intensities.Num());
    Out.AverageMagnitude = sum / n;
    const double var = FMath::Max(0.0, sumsq / n - Out.AverageMagnitude * Out.AverageMagnitude);
    Out.MagnitudeStdDev = FMath::Sqrt(var);
    Out.PeakMagnitude = mx;
    Out.Summary = FString::Printf(TEXT("Avg=%.3f, Peak=%.3f, Std=%.3f"), Out.AverageMagnitude, Out.PeakMagnitude, Out.MagnitudeStdDev);
    return Out;
}

