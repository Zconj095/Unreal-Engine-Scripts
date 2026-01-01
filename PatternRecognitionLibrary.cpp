#include "PatternRecognitionLibrary.h"

FString UPatternRecognitionLibrary::ToLabel_Domain(EPatternDomain Domain)
{
    switch (Domain)
    {
    case EPatternDomain::Brainwave: return TEXT("Brainwave Patterns");
    case EPatternDomain::Energy:    return TEXT("Energy Patterns");
    case EPatternDomain::Speech:    return TEXT("Speech Patterns");
    case EPatternDomain::Thought:   return TEXT("Thought Patterns");
    default:                        return TEXT("Vocal Patterns");
    }
}

FString UPatternRecognitionLibrary::ToLabel_Pattern(EDetectedSeriesPattern Pattern)
{
    switch (Pattern)
    {
    case EDetectedSeriesPattern::Stationary:  return TEXT("Stationary");
    case EDetectedSeriesPattern::TrendUp:     return TEXT("Trend Up");
    case EDetectedSeriesPattern::TrendDown:   return TEXT("Trend Down");
    case EDetectedSeriesPattern::Oscillatory: return TEXT("Oscillatory");
    case EDetectedSeriesPattern::Periodic:    return TEXT("Periodic");
    case EDetectedSeriesPattern::Bursty:      return TEXT("Bursty");
    default:                                  return TEXT("Unknown");
    }
}

FSeriesStats UPatternRecognitionLibrary::ComputeSeriesStats(const TArray<double>& Values, double SampleRateHz)
{
    FSeriesStats S; S.Count = Values.Num();
    if (Values.Num() == 0) return S;

    double sum = 0.0, sumsq = 0.0;
    S.Min = Values[0]; S.Max = Values[0];
    for (int32 i=0;i<Values.Num();++i)
    {
        const double v = Values[i];
        sum += v; sumsq += v*v; S.Min = FMath::Min(S.Min, v); S.Max = FMath::Max(S.Max, v);
        if (i>0) { if ((Values[i-1] >= 0.0 && v < 0.0) || (Values[i-1] < 0.0 && v >= 0.0)) S.ZeroCrossRate += 1.0; }
    }
    S.ZeroCrossRate /= FMath::Max(1, Values.Num()-1);
    S.Mean = sum / double(Values.Num());
    const double var = FMath::Max(0.0, sumsq / double(Values.Num()) - S.Mean*S.Mean);
    S.StdDev = FMath::Sqrt(var);
    S.Range = S.Max - S.Min;
    S.RMS = FMath::Sqrt(sumsq / double(Values.Num()));
    S.TotalEnergy = sumsq;

    // Trend slope via least squares over indices 0..N-1
    {
        const int32 N = Values.Num();
        const double n = double(N);
        const double sumx = 0.5*(n-1.0)*n; // sum 0..N-1
        const double sumx2 = (n-1.0)*n*(2.0*n-1.0)/6.0; // sum i^2
        double sumy = 0.0, sumxy = 0.0;
        for (int32 i=0;i<N;++i) { const double x = double(i); const double y = Values[i]; sumy += y; sumxy += x*y; }
        const double denom = (n*sumx2 - sumx*sumx);
        S.TrendSlope = (denom != 0.0) ? (n*sumxy - sumx*sumy)/denom : 0.0;
    }

    // Autocorrelation peak (normalized) over lags 1..N/4
    {
        const int32 N = Values.Num();
        const int32 MaxLag = FMath::Max(1, N/4);
        double bestR = 0.0; int32 bestLag = 0;
        if (S.StdDev > 0.0)
        {
            for (int32 lag=1; lag<=MaxLag; ++lag)
            {
                double num=0.0; int32 count=0;
                for (int32 i=lag; i<N; ++i) { num += (Values[i]-S.Mean)*(Values[i-lag]-S.Mean); ++count; }
                const double denom = double(count) * var; // var = std^2
                const double r = (denom>0.0) ? (num/denom) : 0.0;
                if (r > bestR) { bestR = r; bestLag = lag; }
            }
        }
        S.AutocorrPeak = bestR; S.AutocorrLag = bestLag;
    }

    // Dominant frequency approximation using zero-cross rate
    if (SampleRateHz > 0.0)
    {
        S.DominantFrequencyHz = 0.5 * S.ZeroCrossRate * SampleRateHz; // rough estimate
    }

    return S;
}

FPatternDetection UPatternRecognitionLibrary::DetectSeriesPattern(const FSeriesStats& Stats, EPatternDomain Domain)
{
    FPatternDetection D; D.Pattern = EDetectedSeriesPattern::Unknown;
    const double std = Stats.StdDev;
    const double slope = Stats.TrendSlope;
    const double zcr = Stats.ZeroCrossRate;
    const double r = Stats.AutocorrPeak;

    // Thresholds tuned per domain (rough)
    double slopeThresh = 0.0; double zcrOsc = 0.15; double rPeriodic = 0.5; double rOsc = 0.3; double burstFracThresh = 0.1;
    switch (Domain)
    {
    case EPatternDomain::Brainwave: slopeThresh = 0.002; zcrOsc = 0.20; rPeriodic = 0.6; rOsc = 0.35; break;
    case EPatternDomain::Energy:    slopeThresh = 0.01;  zcrOsc = 0.10; rPeriodic = 0.5; rOsc = 0.30; break;
    case EPatternDomain::Speech:    slopeThresh = 0.005; zcrOsc = 0.18; rPeriodic = 0.5; rOsc = 0.30; break;
    case EPatternDomain::Thought:   slopeThresh = 0.005; zcrOsc = 0.18; rPeriodic = 0.5; rOsc = 0.30; break;
    case EPatternDomain::Vocal:     slopeThresh = 0.005; zcrOsc = 0.18; rPeriodic = 0.5; rOsc = 0.30; break;
    }

    // Stationary (low variance relative to range) and near-zero slope
    if (std <= KINDA_SMALL_NUMBER && FMath::Abs(slope) < slopeThresh)
    {
        D.Pattern = EDetectedSeriesPattern::Stationary;
        D.Rationale = TEXT("Low variance and negligible slope.");
        return D;
    }

    if (FMath::Abs(slope) >= slopeThresh)
    {
        D.Pattern = (slope > 0.0) ? EDetectedSeriesPattern::TrendUp : EDetectedSeriesPattern::TrendDown;
        D.Rationale = FString::Printf(TEXT("Trend slope = %.6f per sample."), slope);
        // Note: continue to check for strong periodic behavior; if present, override below
    }

    if (zcr >= zcrOsc && r >= rPeriodic)
    {
        D.Pattern = EDetectedSeriesPattern::Periodic;
        D.Rationale = FString::Printf(TEXT("High autocorrelation (r=%.2f) with oscillatory zero-crossing (ZCR=%.2f)."), r, zcr);
        return D;
    }
    else if (zcr >= zcrOsc && r >= rOsc)
    {
        D.Pattern = EDetectedSeriesPattern::Oscillatory;
        D.Rationale = FString::Printf(TEXT("Oscillatory: autocorr r=%.2f, ZCR=%.2f."), r, zcr);
        return D;
    }

    // Bursty: many samples above mean + 2*std
    {
        // Can't compute without raw series here; so approximate using range vs std heuristics
        if (Stats.Range > 4.0 * std)
        {
            D.Pattern = EDetectedSeriesPattern::Bursty;
            D.Rationale = TEXT("Large range relative to standard deviation suggests bursts/outliers.");
            return D;
        }
    }

    if (D.Pattern == EDetectedSeriesPattern::Unknown)
    {
        D.Pattern = EDetectedSeriesPattern::Stationary;
        D.Rationale = TEXT("Defaulting to stationary (no strong trend or oscillation detected).");
    }
    return D;
}

