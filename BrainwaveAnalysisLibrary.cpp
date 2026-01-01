// BrainwaveAnalysisLibrary.cpp
#include "BrainwaveAnalysisLibrary.h"
#include "Algo/Sort.h"
#include <cmath>
#include <cfloat>
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

// --- Static data helpers -----------------------------------------------------

TMap<FString, FChakraData> UBrainwaveAnalysisLibrary::GetDefaultChakras()
{
    TMap<FString, FChakraData> Out;

    auto Make = [](const TCHAR* Name, double Freq, const TCHAR* Color, const TCHAR* Element, const TCHAR* Location, TArray<FString> Props)
    {
        FChakraData D;
        D.Name = Name;
        D.Frequency = Freq;
        D.ColorHex = Color;
        D.Element = Element;
        D.Location = Location;
        D.Properties = MoveTemp(Props);
        return D;
    };

    Out.Add(TEXT("Root"),        Make(TEXT("Root"),        35.0,  TEXT("#FF0000"), TEXT("Earth"),  TEXT("Base of spine"),      {TEXT("Grounding"), TEXT("Survival"), TEXT("Stability"), TEXT("Security")}));
    Out.Add(TEXT("Sacral"),      Make(TEXT("Sacral"),      150.0, TEXT("#FF8800"), TEXT("Water"),  TEXT("Lower abdomen"),     {TEXT("Creativity"), TEXT("Sexuality"), TEXT("Emotion"), TEXT("Pleasure")}));
    Out.Add(TEXT("Solar_Plexus"),Make(TEXT("Solar Plexus"),350.0, TEXT("#FFFF00"), TEXT("Fire"),   TEXT("Upper abdomen"),     {TEXT("Personal power"), TEXT("Confidence"), TEXT("Will"), TEXT("Transformation")}));
    Out.Add(TEXT("Heart"),       Make(TEXT("Heart"),       550.0, TEXT("#00FF00"), TEXT("Air"),    TEXT("Center of chest"),    {TEXT("Love"), TEXT("Compassion"), TEXT("Connection"), TEXT("Healing")}));
    Out.Add(TEXT("Throat"),      Make(TEXT("Throat"),      750.0, TEXT("#0088FF"), TEXT("Sound"),  TEXT("Throat"),             {TEXT("Communication"), TEXT("Truth"), TEXT("Expression"), TEXT("Clarity")}));
    Out.Add(TEXT("Third_Eye"),   Make(TEXT("Third Eye"),   950.0, TEXT("#4400FF"), TEXT("Light"),  TEXT("Forehead"),          {TEXT("Intuition"), TEXT("Wisdom"), TEXT("Vision"), TEXT("Insight")}));
    Out.Add(TEXT("Crown"),       Make(TEXT("Crown"),       1150.0,TEXT("#8800FF"), TEXT("Thought"),TEXT("Top of head"),       {TEXT("Spirituality"), TEXT("Connection to divine"), TEXT("Enlightenment"), TEXT("Unity")}));

    return Out;
}

TMap<FString, FVector2D> UBrainwaveAnalysisLibrary::GetFrequencyRanges()
{
    TMap<FString, FVector2D> Ranges;
    Ranges.Add(TEXT("Delta"),       FVector2D(0.5, 4.0));
    Ranges.Add(TEXT("Theta"),       FVector2D(4.0, 8.0));
    Ranges.Add(TEXT("Alpha"),       FVector2D(8.0, 13.0));
    Ranges.Add(TEXT("Beta"),        FVector2D(13.0, 30.0));
    Ranges.Add(TEXT("Gamma"),       FVector2D(30.0, 100.0));
    Ranges.Add(TEXT("High_Gamma"),  FVector2D(100.0, 200.0));
    Ranges.Add(TEXT("Ultra_Gamma"), FVector2D(200.0, 1000.0));
    Ranges.Add(TEXT("Hyper_Gamma"), FVector2D(1000.0, 5000.0));
    return Ranges;
}

TArray<FNamedStringList> UBrainwaveAnalysisLibrary::GetCorticalAssociations()
{
    TArray<FNamedStringList> Out;
    Out.Add({TEXT("Alpha"),       {TEXT("Occipital Lobe"), TEXT("Parietal Lobe")}});
    Out.Add({TEXT("Beta"),        {TEXT("Frontal Lobe"), TEXT("Temporal Lobe")}});
    Out.Add({TEXT("Theta"),       {TEXT("Temporal Lobe"), TEXT("Parietal Lobe"), TEXT("Hippocampus")}});
    Out.Add({TEXT("Delta"),       {TEXT("Frontal Lobe"), TEXT("Occipital Lobe"), TEXT("Thalamus")}});
    Out.Add({TEXT("Gamma"),       {TEXT("All Lobes"), TEXT("Thalamo-cortical circuits")}});
    Out.Add({TEXT("High_Gamma"),  {TEXT("Prefrontal Cortex"), TEXT("Superior Temporal Gyrus")}});
    Out.Add({TEXT("Ultra_Gamma"), {TEXT("Thalamus"), TEXT("Brainstem"), TEXT("Cerebral Cortex")}});
    Out.Add({TEXT("Hyper_Gamma"), {TEXT("Quantum neural networks"), TEXT("Microtubules"), TEXT("Consciousness networks")}});
    return Out;
}

TArray<FNamedStringList> UBrainwaveAnalysisLibrary::GetActivityAssociations()
{
    TArray<FNamedStringList> Out;
    Out.Add({TEXT("Alpha"),       {TEXT("Relaxation"), TEXT("Reduced anxiety"), TEXT("Creativity"), TEXT("Wakeful rest")}});
    Out.Add({TEXT("Beta"),        {TEXT("Alertness"), TEXT("Concentration"), TEXT("Problem-solving"), TEXT("Active thinking")}});
    Out.Add({TEXT("Theta"),       {TEXT("Deep relaxation"), TEXT("Daydreaming"), TEXT("Meditation"), TEXT("Memory consolidation")}});
    Out.Add({TEXT("Delta"),       {TEXT("Deep sleep"), TEXT("Unconsciousness"), TEXT("Healing"), TEXT("Regeneration")}});
    Out.Add({TEXT("Gamma"),       {TEXT("Enhanced sensory processing"), TEXT("Information binding"), TEXT("Consciousness")}});
    Out.Add({TEXT("High_Gamma"),  {TEXT("Advanced cognitive processing"), TEXT("Heightened awareness")}});
    Out.Add({TEXT("Ultra_Gamma"), {TEXT("Hyper-synchronization"), TEXT("Global neural binding"), TEXT("Peak cognitive performance")}});
    Out.Add({TEXT("Hyper_Gamma"), {TEXT("Transcendent consciousness"), TEXT("Unity experiences"), TEXT("Non-local awareness")}});
    return Out;
}

// --- Core computations -------------------------------------------------------

FQuantumMetrics UBrainwaveAnalysisLibrary::CalculateQuantumCoherence(double FrequencyHz, double Amplitude)
{
    FQuantumMetrics Q;
    FrequencyHz = FMath::Max(0.0, FrequencyHz);
    const double Energy = PlanckConstant * FrequencyHz;
    const double kB = 1.381e-23; // J/K
    const double T = 310.0;      // body temperature K

    Q.EnergyJoules = Energy;
    Q.CoherenceFactor = FMath::Exp(-Energy / (kB * T));
    Q.DecoherenceTimeSeconds = (FrequencyHz > 0.0) ? (1.0 / (2.0 * PI * FrequencyHz)) : 0.0;
    Q.QuantumPhase = FMath::Fmod(2.0 * PI * FrequencyHz * Amplitude, 2.0 * PI);
    Q.QuantumNumber = static_cast<int64>(Energy / PlanckConstant); // ~ Frequency in Hz
    Q.TemperatureKelvin = (Energy > 0.0) ? (Energy / kB) : 0.0;
    return Q;
}

FDNAResonance UBrainwaveAnalysisLibrary::CalculateDNAResonance(double FrequencyHz, double DNAFreq)
{
    FDNAResonance R;
    R.BaseFrequency = FrequencyHz;
    R.DNAWavelengthMeters = 3.4e-9;

    // Harmonics 1..10
    R.Harmonics.Reserve(10);
    for (int i = 1; i <= 10; ++i)
    {
        R.Harmonics.Add(FrequencyHz * static_cast<double>(i));
    }

    // Resonance ratios and optimal index
    int32 ClosestIdx = 0;
    double ClosestDelta = DBL_MAX;
    for (int32 i = 0; i < R.Harmonics.Num(); ++i)
    {
        const double Harm = R.Harmonics[i];
        const double Ratio = (Harm != 0.0) ? (DNAFreq / Harm) : 0.0;
        R.ResonanceRatios.Add(Ratio);
        const double RoundRatio = FMath::RoundToDouble(Ratio);
        const double Delta = FMath::Abs(Ratio - RoundRatio);
        if (Delta < ClosestDelta)
        {
            ClosestDelta = Delta;
            ClosestIdx = i;
        }
    }

    R.OptimalHarmonic = (R.Harmonics.IsValidIndex(ClosestIdx)) ? R.Harmonics[ClosestIdx] : 0.0;

    // EM wavelength and interference estimate
    const double EMWavelength = (FrequencyHz > 0.0) ? (SpeedOfLight / FrequencyHz) : 0.0;
    R.EMWavelengthMeters = EMWavelength;
    R.InterferenceRatio = (R.DNAWavelengthMeters > 0.0) ? (EMWavelength / R.DNAWavelengthMeters) : 0.0;
    R.StandingWaveNodes = static_cast<int32>(R.InterferenceRatio / 2.0);

    const double BestRatio = R.ResonanceRatios.IsValidIndex(ClosestIdx) ? R.ResonanceRatios[ClosestIdx] : 0.0;
    const double BestDiff = FMath::Abs(BestRatio - FMath::RoundToDouble(BestRatio));
    R.ResonanceStrength = 1.0 / (1.0 + BestDiff);
    return R;
}

FString UBrainwaveAnalysisLibrary::ClassifyFrequency(double FrequencyHz)
{
    const TMap<FString, FVector2D> Ranges = GetFrequencyRanges();
    for (const auto& KVP : Ranges)
    {
        const FVector2D Range = KVP.Value;
        if (FrequencyHz >= Range.X && FrequencyHz < Range.Y)
        {
            return KVP.Key;
        }
    }
    return TEXT("Unknown");
}

FWavelengthMetrics UBrainwaveAnalysisLibrary::CalculateWavelengthMetrics(double PulseFrequencyHz, double PulseAmplitude, double MagneticFieldDirection)
{
    FWavelengthMetrics M;
    PulseFrequencyHz = FMath::Max(0.0, PulseFrequencyHz);

    M.PulseAmplitude = PulseAmplitude;
    M.PulseFrequency = PulseFrequencyHz;
    M.MagneticFieldDirection = MagneticFieldDirection;
    M.Wavelength = (PulseFrequencyHz > 0.0) ? (SpeedOfLight / PulseFrequencyHz) : 0.0;

    M.WavelengthClass = ClassifyFrequency(PulseFrequencyHz);
    const TMap<FString, FVector2D> Ranges = GetFrequencyRanges();
    if (const FVector2D* Range = Ranges.Find(M.WavelengthClass))
    {
        M.FrequencyRange = *Range;
    }

    // Category by wavelength
    if (M.Wavelength <= 100.0)
    {
        M.WavelengthCategory = TEXT("High Frequency (Low Wavelength)");
    }
    else if (M.Wavelength <= 1000.0)
    {
        M.WavelengthCategory = TEXT("Medium Frequency (Medium Wavelength)");
    }
    else
    {
        M.WavelengthCategory = TEXT("Low Frequency (High Wavelength)");
    }

    // Pattern by amplitude
    if (PulseAmplitude < 0.3)
    {
        M.WavelengthPattern = TEXT("Low Amplitude (Stable)");
    }
    else if (PulseAmplitude < 0.7)
    {
        M.WavelengthPattern = TEXT("Medium Amplitude (Variable)");
    }
    else
    {
        M.WavelengthPattern = TEXT("High Amplitude (Dynamic)");
    }

    // Associations
    {
        const TArray<FNamedStringList> Assoc = GetCorticalAssociations();
        bool bFound = false;
        for (const FNamedStringList& E : Assoc)
        {
            if (E.Name == M.WavelengthClass)
            {
                M.CorticalRegions = E.Values;
                bFound = true;
                break;
            }
        }
        if (!bFound)
        {
            M.CorticalRegions = { TEXT("Unknown") };
        }
    }

    {
        const TArray<FNamedStringList> Acts = GetActivityAssociations();
        bool bFound = false;
        for (const FNamedStringList& E : Acts)
        {
            if (E.Name == M.WavelengthClass)
            {
                M.AssociatedActivities = E.Values;
                bFound = true;
                break;
            }
        }
        if (!bFound)
        {
            M.AssociatedActivities = { TEXT("Unknown") };
        }
    }

    M.PowerEstimate = PulseAmplitude * PulseAmplitude;
    M.EnergyDensity = (M.Wavelength != 0.0) ? (M.PowerEstimate / M.Wavelength) : 0.0;

    // Enhanced: embed quantum + DNA
    M.Quantum = CalculateQuantumCoherence(PulseFrequencyHz, PulseAmplitude);
    M.DNA = CalculateDNAResonance(PulseFrequencyHz);

    return M;
}

// --- Chakra system analysis --------------------------------------------------

TArray<double> UBrainwaveAnalysisLibrary::GetFrequenciesFromMap(const TMap<FString, FChakraData>& Chakras)
{
    TArray<double> Freqs;
    Freqs.Reserve(Chakras.Num());
    for (const auto& KVP : Chakras)
    {
        Freqs.Add(KVP.Value.Frequency);
    }
    return Freqs;
}

static double StdDev(const TArray<double>& Values)
{
    if (Values.Num() <= 1) return 0.0;
    double Mean = 0.0;
    for (double V : Values) Mean += V;
    Mean /= static_cast<double>(Values.Num());
    double Acc = 0.0;
    for (double V : Values)
    {
        const double D = V - Mean;
        Acc += D * D;
    }
    return FMath::Sqrt(Acc / static_cast<double>(Values.Num()));
}

TArray<FString> UBrainwaveAnalysisLibrary::GenerateRecommendations(const TMap<FString, FChakraAnalysis>& Analyses)
{
    TArray<FString> Recs;
    for (const auto& KVP : Analyses)
    {
        const FString& ChakraName = KVP.Key;
        const FChakraAnalysis& A = KVP.Value;
        const double Activation = A.ActivationScore;
        const FString ElementLower = A.Chakra.Element.ToLower();
        if (Activation < 0.3)
        {
            Recs.Add(FString::Printf(TEXT("Consider %s element practices for %s chakra"), *ElementLower, *ChakraName));
            if (A.Chakra.Properties.Num() > 0)
            {
                const FString FirstPropLower = A.Chakra.Properties[0].ToLower();
                Recs.Add(FString::Printf(TEXT("Focus on %s exercises"), *FirstPropLower));
            }
        }
        else if (Activation > 0.8)
        {
            Recs.Add(FString::Printf(TEXT("%s chakra is highly active - maintain balance"), *ChakraName));
        }
    }
    return Recs;
}

FChakraSystemResult UBrainwaveAnalysisLibrary::AnalyzeChakraSystem(const TArray<double>& OverrideFrequencies)
{
    const TMap<FString, FChakraData> Chakras = GetDefaultChakras();
    TArray<FString> Keys;
    Chakras.GetKeys(Keys);
    Keys.Sort([](const FString& A, const FString& B){ return A < B; });

    FChakraSystemResult Out;

    // Build analyses, optionally substituting frequencies by index order
    for (int32 i = 0; i < Keys.Num(); ++i)
    {
        const FString& Name = Keys[i];
        const FChakraData* Data = Chakras.Find(Name);
        if (!Data) continue;

        const double Freq = (OverrideFrequencies.IsValidIndex(i)) ? OverrideFrequencies[i] : Data->Frequency;

        FChakraAnalysis A;
        A.Chakra = *Data;
        A.Frequency = Freq;
        A.Quantum = CalculateQuantumCoherence(Freq);
        A.DNA = CalculateDNAResonance(Freq);
        A.Brainwave = CalculateWavelengthMetrics(Freq);
        A.EnergyLevel = A.Quantum.EnergyJoules;
        A.ActivationScore = A.Quantum.CoherenceFactor * A.DNA.ResonanceStrength;

        Out.Individual.Add(Name, A);

        Out.System.TotalEnergy += A.Quantum.EnergyJoules;
        Out.System.CoherenceSum += A.Quantum.CoherenceFactor;
        Out.System.EnergyDistribution.Add(Name, A.Quantum.EnergyJoules);
    }

    // Balance and dominant chakra
    TArray<double> Energies;
    Out.System.EnergyDistribution.GenerateValueArray(Energies);
    if (Energies.Num() > 0)
    {
        double Mean = 0.0;
        for (double E : Energies) Mean += E;
        Mean /= static_cast<double>(Energies.Num());
        const double SD = StdDev(Energies);
        Out.System.BalanceScore = (Mean > 0.0) ? (1.0 - (SD / Mean)) : 0.0;
    }

    // Dominant by max energy
    double BestEnergy = -1.0;
    FString BestName;
    for (const auto& KVP : Out.System.EnergyDistribution)
    {
        if (KVP.Value > BestEnergy)
        {
            BestEnergy = KVP.Value;
            BestName = KVP.Key;
        }
    }
    Out.System.DominantChakra = BestName;

    Out.Recommendations = GenerateRecommendations(Out.Individual);
    return Out;
}

FChakraSystemResult UBrainwaveAnalysisLibrary::AnalyzeDefaultChakraSystem()
{
    TArray<double> Empty;
    return AnalyzeChakraSystem(Empty);
}

// --- Frequency sweep and interaction (quasi-particle analog) -----------------

TArray<double> UBrainwaveAnalysisLibrary::GenerateFrequencyRange(double MinFreq, double MaxFreq, int32 NumFrequencies, double LogPortion)
{
    TArray<double> Out;
    if (NumFrequencies <= 0 || MinFreq <= 0.0 || MaxFreq <= MinFreq)
    {
        return Out;
    }

    const int32 NumLog = FMath::Clamp(static_cast<int32>(FMath::RoundToDouble(LogPortion * NumFrequencies)), 0, NumFrequencies);
    const int32 NumLin = NumFrequencies - NumLog;

    // Logarithmic spacing
    TArray<double> LogPart;
    if (NumLog > 0)
    {
        LogPart.Reserve(NumLog);
        const double LogMin = FMath::Loge(MinFreq);
        const double LogMax = FMath::Loge(MaxFreq);
        for (int32 i = 0; i < NumLog; ++i)
        {
            const double T = (NumLog == 1) ? 0.0 : static_cast<double>(i) / static_cast<double>(NumLog - 1);
            const double Val = FMath::Exp(LogMin + (LogMax - LogMin) * T);
            LogPart.Add(Val);
        }
    }

    // Linear spacing
    TArray<double> LinPart;
    if (NumLin > 0)
    {
        LinPart.Reserve(NumLin);
        for (int32 i = 0; i < NumLin; ++i)
        {
            const double T = (NumLin == 1) ? 0.0 : static_cast<double>(i) / static_cast<double>(NumLin - 1);
            LinPart.Add(FMath::Lerp(MinFreq, MaxFreq, T));
        }
    }

    // Merge, sort, unique-ish
    Out = LogPart;
    Out.Append(LinPart);
    Out.Sort();
    // Remove near-duplicates
    TArray<double> Unique;
    Unique.Reserve(Out.Num());
    double Last = -1.0;
    const double Eps = 1e-9;
    for (double V : Out)
    {
        if (Last < 0.0 || FMath::Abs(V - Last) > Eps)
        {
            Unique.Add(V);
            Last = V;
        }
    }
    // Truncate to requested count if needed
    if (Unique.Num() > NumFrequencies)
    {
        Unique.SetNum(NumFrequencies);
    }
    return Unique;
}

FFrequencyBatchResult UBrainwaveAnalysisLibrary::BatchAnalyzeFrequencies(const TArray<double>& Frequencies)
{
    FFrequencyBatchResult R;
    R.Frequencies = Frequencies;
    R.Wavelengths.Reserve(Frequencies.Num());
    R.Classifications.Reserve(Frequencies.Num());
    R.Metrics.Reserve(Frequencies.Num());

    for (double Freq : Frequencies)
    {
        const double W = (Freq > 0.0) ? (SpeedOfLight / Freq) : 0.0;
        R.Wavelengths.Add(W);
        const FString Class = ClassifyFrequency(Freq);
        R.Classifications.Add(Class);
        R.Metrics.Add(CalculateWavelengthMetrics(Freq));
    }
    return R;
}

FChakraInteractionResult UBrainwaveAnalysisLibrary::RunChakraInteraction(const FString& ChakraName, double MinFreq, double MaxFreq, int32 NumFrequencies, int32 Shots)
{
    FChakraInteractionResult Out;
    Out.ChakraName = ChakraName;

    // Generate and analyze frequencies
    Out.Frequencies = GenerateFrequencyRange(MinFreq, MaxFreq, NumFrequencies, 1.0); // use log-only for coverage
    if (Out.Frequencies.Num() == 0)
    {
        Out.Counts.Add(TEXT("0"), Shots/2);
        Out.Counts.Add(TEXT("1"), Shots - Shots/2);
        Out.AverageFieldValue = 0.0;
        return Out;
    }

    // Deterministic field computation using energy density
    double SumField = 0.0;
    Out.FieldValues.Reset(Out.Frequencies.Num());
    for (double Freq : Out.Frequencies)
    {
        const FWavelengthMetrics M = CalculateWavelengthMetrics(Freq);
        Out.FieldValues.Add(M.EnergyDensity);
        SumField += M.EnergyDensity;
    }
    Out.AverageFieldValue = (Out.Frequencies.Num() > 0) ? SumField / static_cast<double>(Out.Frequencies.Num()) : 0.0;

    // Simple counts: split by sign of (EnergyDensity - Average)
    int32 CountOnes = 0;
    for (double V : Out.FieldValues)
    {
        if (V >= Out.AverageFieldValue) ++CountOnes;
    }
    // Scale to requested Shots proportionally
    const double P = (Out.FieldValues.Num() > 0) ? static_cast<double>(CountOnes) / static_cast<double>(Out.FieldValues.Num()) : 0.5;
    const int32 Ones = FMath::Clamp(static_cast<int32>(FMath::RoundToInt(P * Shots)), 0, Shots);
    const int32 Zeros = Shots - Ones;
    Out.Counts.Add(TEXT("0"), Zeros);
    Out.Counts.Add(TEXT("1"), Ones);
    return Out;
}

// --- Seasonal helpers --------------------------------------------------------

static void ComputeMeanStd(const TArray<double>& Data, double& OutMean, double& OutStd)
{
    if (Data.Num() == 0) { OutMean = 0.0; OutStd = 0.0; return; }
    double Sum = 0.0; for (double v : Data) Sum += v; OutMean = Sum / double(Data.Num());
    double Acc = 0.0; for (double v : Data) { double d = v - OutMean; Acc += d*d; }
    OutStd = FMath::Sqrt(Acc / double(Data.Num()));
}

FFrequencyStats UBrainwaveAnalysisLibrary::SummarizeBatch(const FFrequencyBatchResult& Batch)
{
    FFrequencyStats S;
    const int32 N = Batch.Frequencies.Num();
    S.TotalAnalyzed = N;
    if (N == 0) return S;

    double FMin = DBL_MAX, FMax = -DBL_MAX;
    double WMin = DBL_MAX, WMax = -DBL_MAX;
    for (int32 i = 0; i < N; ++i)
    {
        const double f = Batch.Frequencies[i];
        const double w = Batch.Wavelengths.IsValidIndex(i) ? Batch.Wavelengths[i] : 0.0;
        FMin = FMath::Min(FMin, f); FMax = FMath::Max(FMax, f);
        WMin = FMath::Min(WMin, w); WMax = FMath::Max(WMax, w);
        S.BandDistribution.FindOrAdd(Batch.Classifications.IsValidIndex(i) ? Batch.Classifications[i] : TEXT("Unknown"))++;
    }
    S.FreqMin = FMin; S.FreqMax = FMax;
    S.WaveMin = WMin; S.WaveMax = WMax;

    double FMean = 0.0, FStd = 0.0; ComputeMeanStd(Batch.Frequencies, FMean, FStd);
    double WMean = 0.0, WStd = 0.0; ComputeMeanStd(Batch.Wavelengths, WMean, WStd);
    S.FreqMean = FMean; S.FreqStd = FStd;
    S.WaveMean = WMean; S.WaveStd = WStd;

    for (const auto& KVP : S.BandDistribution)
    {
        const double Pct = (N > 0) ? (double(KVP.Value) / double(N)) * 100.0 : 0.0;
        S.BandPercentages.Add(KVP.Key, Pct);
    }
    return S;
}

double UBrainwaveAnalysisLibrary::SeasonalModulationAutumn(int32 DayOfYear)
{
    const double SeasonalPhase = (double(DayOfYear) - 80.0) / 365.25;
    return 0.5 * (1.0 + FMath::Cos(2.0 * PI * SeasonalPhase));
}

double UBrainwaveAnalysisLibrary::WinterModulation(double FrequencyHz)
{
    return FMath::Tanh(FrequencyHz / 100.0);
}

static bool ParseRangeHz(const FString& Field, FVector2D& OutRange)
{
    FString Trim = Field;
    Trim.TrimStartAndEndInline();
    // Drop any trailing tokens (e.g., "Hz")
    int32 SpaceIdx;
    if (Trim.FindChar(TEXT(' '), SpaceIdx))
    {
        Trim = Trim.Left(SpaceIdx);
    }
    if (Trim.Contains(TEXT("-")))
    {
        FString Lo, Hi;
        if (Trim.Split(TEXT("-"), &Lo, &Hi))
        {
            OutRange.X = FCString::Atod(*Lo);
            OutRange.Y = FCString::Atod(*Hi);
            return OutRange.X <= OutRange.Y;
        }
    }
    else
    {
        const double V = FCString::Atod(*Trim);
        OutRange = FVector2D(V, V);
        return true;
    }
    return false;
}

bool UBrainwaveAnalysisLibrary::ParseCSVChakraRow(const FString& FilePath, const FString& ChakraQuery, FVector2D& OutRangeHz, FString& OutDescription)
{
    OutRangeHz = FVector2D(0,0);
    OutDescription.Reset();
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        return false;
    }

    TArray<FString> Lines;
    Content.ParseIntoArrayLines(Lines, true);
    if (Lines.Num() <= 1) return false;

    // Assume header line contains: Chakra,EM Field Frequency,Ki
    for (int32 i = 1; i < Lines.Num(); ++i)
    {
        const FString& Line = Lines[i];
        TArray<FString> Cells;
        Line.ParseIntoArray(Cells, TEXT(","), false);
        if (Cells.Num() < 3) continue;
        const FString ChakraCell = Cells[0].TrimStartAndEnd();
        if (ChakraCell.Equals(ChakraQuery, ESearchCase::IgnoreCase))
        {
            const FString FreqCell = Cells[1].TrimStartAndEnd();
            const FString KiCell = Cells[2].TrimStartAndEnd();
            if (!ParseRangeHz(FreqCell, OutRangeHz)) return false;
            OutDescription = KiCell;
            return true;
        }
    }
    return false;
}

FSpringInteractionResult UBrainwaveAnalysisLibrary::AnalyzeChakraSpringFromCSV(const FString& FilePath, const FString& ChakraName, const FString& Season, int32 Samples)
{
    FSpringInteractionResult R;
    R.Chakra = ChakraName; R.Season = Season;
    if (!ParseCSVChakraRow(FilePath, ChakraName, R.FrequencyRangeHz, R.Description))
    {
        return R;
    }

    const TArray<double> Freqs = GenerateFrequencyRange(R.FrequencyRangeHz.X, R.FrequencyRangeHz.Y, Samples, 1.0);
    const FFrequencyBatchResult Batch = BatchAnalyzeFrequencies(Freqs);
    R.Summary = SummarizeBatch(Batch);

    // Quantum-like sample frequency: use FRandomStream to simulate 8-bit randomness
    FRandomStream RNG(FPlatformTime::Cycles());
    const double U = RNG.FRand();
    R.SampleFrequencyHz = R.FrequencyRangeHz.X + U * (R.FrequencyRangeHz.Y - R.FrequencyRangeHz.X);
    R.Metrics = CalculateWavelengthMetrics(R.SampleFrequencyHz);
    return R;
}

FSummerInteractionResult UBrainwaveAnalysisLibrary::AnalyzeChakraSummer(const FString& ChakraName, double BaseFrequencyHz, int32 NumPoints, int32 Shots)
{
    FSummerInteractionResult R; R.Chakra = ChakraName; R.Shots = Shots;
    // Build small window around base frequency
    const double Half = 0.5;
    const TArray<double> Freqs = GenerateFrequencyRange(BaseFrequencyHz - Half, BaseFrequencyHz + Half, NumPoints, 0.0);
    const FFrequencyBatchResult Batch = BatchAnalyzeFrequencies(Freqs);
    R.Summary = SummarizeBatch(Batch);

    // Phase = base_frequency mod 2pi, probability of 1 = sin^2(theta/2)
    const double Theta = FMath::Fmod(BaseFrequencyHz, 2.0 * PI);
    const double P1 = FMath::Square(FMath::Sin(0.5 * Theta));
    const int32 Ones = FMath::Clamp(static_cast<int32>(FMath::RoundToDouble(P1 * double(Shots))), 0, Shots);
    const int32 Zeros = Shots - Ones;
    R.Counts.Add(TEXT("0"), Zeros);
    R.Counts.Add(TEXT("1"), Ones);
    return R;
}

FAutumnAnalysisResult UBrainwaveAnalysisLibrary::AnalyzeChakraAutumn(const FString& ChakraName, double BaseFrequencyHz, int32 DayOfYear, int32 Shots)
{
    FAutumnAnalysisResult R; R.Chakra = ChakraName; R.DayOfYear = DayOfYear;
    R.SeasonFactor = SeasonalModulationAutumn(DayOfYear);
    R.SeasonalFrequencyHz = BaseFrequencyHz * R.SeasonFactor;
    R.Metrics = CalculateWavelengthMetrics(R.SeasonalFrequencyHz);

    // theta = 2*pi*freq/1000, p1 = sin^2(theta/2)
    const double Theta = 2.0 * PI * R.SeasonalFrequencyHz / 1000.0;
    const double P1 = FMath::Square(FMath::Sin(0.5 * Theta));
    R.QuantumP1 = P1; // probability; caller can scale by shots if needed
    return R;
}

FWinterAnalysisResult UBrainwaveAnalysisLibrary::AnalyzeChakraWinter(const FString& ChakraName, double BaseFrequencyHz)
{
    FWinterAnalysisResult R; R.Chakra = ChakraName; R.BaseFrequencyHz = BaseFrequencyHz;
    R.Modulation = WinterModulation(BaseFrequencyHz);
    R.Amplitude = 0.5 * R.Modulation;
    R.Metrics = CalculateWavelengthMetrics(BaseFrequencyHz, R.Amplitude, 1.0);

    // Quantum strength analog via single-qubit rotation: angle scales with frequency
    const double Angle = (FMath::Fmod(BaseFrequencyHz, 512.0) / 512.0) * PI;
    const double P1 = FMath::Square(FMath::Sin(0.5 * Angle)) * 2.0; // scale to emphasize
    R.QuantumStrength = FMath::Clamp(P1, 0.0, 1.0);
    return R;
}

double UBrainwaveAnalysisLibrary::SolsticeResonance(double ChakraFrequencyHz, double ReferenceHz, double SolsticeAngleDeg)
{
    const double AngleRad = FMath::DegreesToRadians(SolsticeAngleDeg);
    const double Theta = AngleRad * (ChakraFrequencyHz / FMath::Max(ReferenceHz, KINDA_SMALL_NUMBER));
    // Probability of |1> for Ry(Theta)|0> is sin^2(Theta/2)
    return FMath::Square(FMath::Sin(0.5 * Theta));
}

FSolsticeAnalysisResult UBrainwaveAnalysisLibrary::AnalyzeChakraSolstice(const FString& ChakraName, double ChakraFrequencyHz, double ReferenceHz, double SolsticeAngleDeg)
{
    FSolsticeAnalysisResult R;
    R.Chakra = ChakraName;
    R.ChakraFrequencyHz = ChakraFrequencyHz;
    R.SolsticeAngleDeg = SolsticeAngleDeg;
    R.ReferenceFrequencyHz = ReferenceHz;

    R.ResonanceAmplitude = SolsticeResonance(ChakraFrequencyHz, ReferenceHz, SolsticeAngleDeg);
    const double AngleRad = FMath::DegreesToRadians(SolsticeAngleDeg);
    R.MagneticFieldDirection = FMath::Cos(AngleRad);

    R.Metrics = CalculateWavelengthMetrics(ChakraFrequencyHz, R.ResonanceAmplitude, R.MagneticFieldDirection);

    R.SimpleReport = FString::Printf(
        TEXT("Chakra: %s\nSolstice Resonance: %.4f\nFrequency: %.2f Hz\nClass: %s\nWavelength: %.3e m\n"),
        *ChakraName,
        R.ResonanceAmplitude,
        ChakraFrequencyHz,
        *R.Metrics.WavelengthClass,
        R.Metrics.Wavelength
    );

    return R;
}

// --- Speaking style monitoring (Throat chakra) -------------------------------

static TMap<FString, double> MakeStyleMap()
{
    TMap<FString, double> M;
    M.Add(TEXT("long_breathed"),    1.1);
    M.Add(TEXT("long_winded"),      1.0);
    M.Add(TEXT("loud_booming"),     1.5);
    M.Add(TEXT("loud_soft_spoken"), 0.7);
    M.Add(TEXT("manual"),           0.95);
    M.Add(TEXT("melodic"),          1.2);
    M.Add(TEXT("modest"),           0.8);
    return M;
}

TMap<FString, double> UBrainwaveAnalysisLibrary::GetSpeakingStyleFactors()
{
    return MakeStyleMap();
}

static double GetStyleFactor(const FString& Style)
{
    const TMap<FString, double> M = MakeStyleMap();
    if (const double* V = M.Find(Style)) return *V;
    // Try lowercase
    const FString Lower = Style.ToLower();
    if (const double* VL = M.Find(Lower)) return *VL;
    return 1.0;
}

static FString BuildSimpleReport(double FrequencyHz, double Amplitude, const FWavelengthMetrics& Metrics)
{
    return FString::Printf(
        TEXT("Frequency: %.2f Hz\nAmplitude: %.4f\nClass: %s\nCategory: %s\nWavelength: %.3e m\n"),
        FrequencyHz,
        Amplitude,
        *Metrics.WavelengthClass,
        *Metrics.WavelengthCategory,
        Metrics.Wavelength
    );
}

FString UBrainwaveAnalysisLibrary::AnalyzeThroatChakraReport(const FString& Style, double Amplitude, double FrequencyHz)
{
    const double ClampedAmp = FMath::Clamp(Amplitude, 0.0, 1.0);
    const double EffAmp = ClampedAmp; // style can be applied in other API variants
    const FWavelengthMetrics Metrics = CalculateWavelengthMetrics(FrequencyHz, EffAmp, 1.0);
    return BuildSimpleReport(FrequencyHz, EffAmp, Metrics);
}

FSpeakingStyleResult UBrainwaveAnalysisLibrary::AnalyzeThroatChakraEmphasis(const FString& Style, double Amplitude, double FrequencyHz, int32 Shots)
{
    FSpeakingStyleResult R;
    R.Style = Style;
    R.FrequencyHz = FrequencyHz;
    const double Factor = GetStyleFactor(Style);
    R.Amplitude = FMath::Clamp(Amplitude * Factor, 0.0, 1.0);
    R.Metrics = CalculateWavelengthMetrics(FrequencyHz, R.Amplitude, 1.0);
    R.Shots = Shots;

    // Quantum analog: p1 = sin^2(amp/2)
    R.ProbabilityOne = FMath::Square(FMath::Sin(0.5 * R.Amplitude));
    const int32 Ones = FMath::Clamp(static_cast<int32>(FMath::RoundToDouble(R.ProbabilityOne * double(Shots))), 0, Shots);
    const int32 Zeros = Shots - Ones;
    R.Counts.Add(TEXT("0"), Zeros);
    R.Counts.Add(TEXT("1"), Ones);
    R.Report = BuildSimpleReport(FrequencyHz, R.Amplitude, R.Metrics);
    return R;
}

FSpeakingStyleResult UBrainwaveAnalysisLibrary::AnalyzeThroatChakraEmphasisByDuration(const FString& Style, double DurationSeconds, double BaseFrequencyHz, int32 Shots)
{
    FSpeakingStyleResult R;
    R.Style = Style;
    R.FrequencyHz = BaseFrequencyHz;
    // Approximate mean(|sin|) over many cycles = 2/pi
    const double MeanAbsSin = 2.0 / PI; // ~0.6366
    const double Factor = GetStyleFactor(Style);
    R.Amplitude = FMath::Clamp(Factor * MeanAbsSin, 0.0, 1.0);
    R.Metrics = CalculateWavelengthMetrics(BaseFrequencyHz, R.Amplitude, 1.0);
    R.Shots = Shots;

    R.ProbabilityOne = FMath::Square(FMath::Sin(0.5 * R.Amplitude));
    const int32 Ones = FMath::Clamp(static_cast<int32>(FMath::RoundToDouble(R.ProbabilityOne * double(Shots))), 0, Shots);
    const int32 Zeros = Shots - Ones;
    R.Counts.Add(TEXT("0"), Zeros);
    R.Counts.Add(TEXT("1"), Ones);
    R.Report = BuildSimpleReport(BaseFrequencyHz, R.Amplitude, R.Metrics);
    return R;
}

// --- Cognition utilities -----------------------------------------------------

FWavelengthMetrics UBrainwaveAnalysisLibrary::AnalyzeSingleChakraCognition(const FString& ChakraName, double FrequencyHz)
{
    // For cognition we return detailed wavelength metrics; ChakraName is informational only
    return CalculateWavelengthMetrics(FrequencyHz, 0.0001, 1.0);
}

// --- Z-score baseline and pattern tracking ----------------------------------

static FZBaseline GZBaseline;
static bool GbHasBaseline = false;
static double GZThreshold = 2.0;

struct FPatternLogEntry
{
    FString TimestampIso;
    double PulseFrequency = 0.0;
    double PulseAmplitude = 0.0;
    FZScoreResult Z;
    FPatternAnalysisUE Pattern;
};

static TArray<FPatternAnalysisUE> GPatternHistory;
static TArray<FPatternLogEntry> GPatternLog;

static double NowToMinutesSince(const FString& TimestampIso)
{
    FDateTime BaselineTime;
    if (!FDateTime::ParseIso8601(*TimestampIso, BaselineTime)) return 0.0;
    const FTimespan Age = FDateTime::UtcNow() - BaselineTime;
    return Age.GetTotalMinutes();
}

static FStatSummary MakeSummary(const TArray<double>& Values)
{
    FStatSummary S;
    S.Count = Values.Num();
    if (S.Count == 0) return S;
    double Sum = 0.0; for (double v : Values) Sum += v;
    S.Mean = Sum / double(S.Count);
    double Acc = 0.0; for (double v : Values) { double d = v - S.Mean; Acc += d * d; }
    S.Std = (S.Count > 0) ? FMath::Sqrt(Acc / double(S.Count)) : 0.0;
    return S;
}

FZBaseline UBrainwaveAnalysisLibrary::EstablishZBaseline(const TArray<double>& Frequencies, const TArray<double>& Amplitudes)
{
    FZBaseline B;
    if (Frequencies.Num() == 0)
    {
        GbHasBaseline = false;
        GZBaseline = B;
        return B;
    }

    // Frequency summary
    B.Frequency = MakeSummary(Frequencies);

    // Amplitude summary (optional)
    if (Amplitudes.Num() > 0)
    {
        B.bHasAmplitude = true;
        B.Amplitude = MakeSummary(Amplitudes);
    }

    // Band-specific baselines
    const TMap<FString, FVector2D> Ranges = GetFrequencyRanges();
    TMap<FString, TArray<double>> BandBuckets;
    {
        TArray<FString> Keys; Ranges.GetKeys(Keys);
        for (const FString& BandName : Keys)
        {
            BandBuckets.Add(BandName, {});
        }
    }
    for (double f : Frequencies)
    {
        const FString Class = ClassifyFrequency(f);
        if (TArray<double>* Bucket = BandBuckets.Find(Class))
        {
            Bucket->Add(f);
        }
    }
    for (const auto& KVP : BandBuckets)
    {
        FStatSummary S = MakeSummary(KVP.Value);
        S.Percentage = (Frequencies.Num() > 0) ? (double(S.Count) / double(Frequencies.Num())) * 100.0 : 0.0;
        B.BandSummaries.Add(FString::Printf(TEXT("%s_band"), *KVP.Key.ToLower()), S);
    }

    B.TimestampIso = FDateTime::UtcNow().ToIso8601();
    GZBaseline = B;
    GbHasBaseline = true;
    return B;
}

FZScoreResult UBrainwaveAnalysisLibrary::CalculateZScores(double PulseFrequency, double PulseAmplitude)
{
    FZScoreResult Z;
    Z.TimestampIso = FDateTime::UtcNow().ToIso8601();
    Z.InputFrequency = PulseFrequency;
    Z.InputAmplitude = PulseAmplitude;
    if (!GbHasBaseline)
    {
        return Z;
    }
    if (GZBaseline.Frequency.Std > 0.0)
    {
        Z.FrequencyZ = (PulseFrequency - GZBaseline.Frequency.Mean) / GZBaseline.Frequency.Std;
        Z.bFrequencyAnomaly = FMath::Abs(Z.FrequencyZ) > GZThreshold;
    }
    if (GZBaseline.bHasAmplitude && GZBaseline.Amplitude.Std > 0.0)
    {
        Z.AmplitudeZ = (PulseAmplitude - GZBaseline.Amplitude.Mean) / GZBaseline.Amplitude.Std;
        Z.bAmplitudeAnomaly = FMath::Abs(Z.AmplitudeZ) > GZThreshold;
    }

    const FString Band = ClassifyFrequency(PulseFrequency);
    Z.CurrentBand = Band;
    if (!Band.Equals(TEXT("Unknown")))
    {
        const FString Key = FString::Printf(TEXT("%s_band"), *Band.ToLower());
        if (const FStatSummary* BS = GZBaseline.BandSummaries.Find(Key))
        {
            if (BS->Std > 0.0)
            {
                Z.BandZ = (PulseFrequency - BS->Mean) / BS->Std;
                Z.bBandAnomaly = FMath::Abs(Z.BandZ) > GZThreshold;
            }
        }
    }

    int32 Count = 0;
    if (Z.bFrequencyAnomaly) ++Count;
    if (Z.bAmplitudeAnomaly) ++Count;
    Z.AnomalyCount = Count;
    Z.bOverallAnomaly = (Count > 0);
    return Z;
}

FPatternAnalysisUE UBrainwaveAnalysisLibrary::TrackPatternChanges(const FZScoreResult& ZScores)
{
    FPatternAnalysisUE P;
    P.TimestampIso = ZScores.TimestampIso;
    // Defaults are normal/low
    if (ZScores.bOverallAnomaly)
    {
        const double FZ = ZScores.FrequencyZ;
        const double AZ = ZScores.AmplitudeZ;
        if (FMath::Abs(FZ) > 3.0)
        {
            P.PatternType = TEXT("extreme_frequency_deviation");
            P.Severity = TEXT("high");
            P.Description = FString::Printf(TEXT("Extreme frequency deviation (z-score: %.2f)"), FZ);
        }
        else if (FMath::Abs(AZ) > 3.0)
        {
            P.PatternType = TEXT("extreme_amplitude_deviation");
            P.Severity = TEXT("high");
            P.Description = FString::Printf(TEXT("Extreme amplitude deviation (z-score: %.2f)"), AZ);
        }
        else if (FZ > 2.0 && AZ > 2.0)
        {
            P.PatternType = TEXT("high_activity_state");
            P.Severity = TEXT("medium");
            P.Description = TEXT("Both frequency and amplitude elevated");
        }
        else if (FZ < -2.0 && AZ < -2.0)
        {
            P.PatternType = TEXT("low_activity_state");
            P.Severity = TEXT("medium");
            P.Description = TEXT("Both frequency and amplitude suppressed");
        }
        else
        {
            P.PatternType = TEXT("mild_anomaly");
            P.Severity = TEXT("low");
            P.Description = TEXT("Mild deviation from baseline");
        }
    }

    GPatternHistory.Add(P);
    if (GPatternHistory.Num() > 1000)
    {
        GPatternHistory.RemoveAt(0, GPatternHistory.Num() - 1000);
    }
    return P;
}

void UBrainwaveAnalysisLibrary::LogBrainwavePattern(double PulseFrequency, double PulseAmplitude, const FZScoreResult& ZScores, const FPatternAnalysisUE& Pattern)
{
    FPatternLogEntry E;
    E.TimestampIso = FDateTime::UtcNow().ToIso8601();
    E.PulseFrequency = PulseFrequency;
    E.PulseAmplitude = PulseAmplitude;
    E.Z = ZScores;
    E.Pattern = Pattern;
    GPatternLog.Add(E);
    if (GPatternLog.Num() > 5000)
    {
        GPatternLog.RemoveAt(0, GPatternLog.Num() - 5000);
    }

    // Log significant messages to UE log
    if (Pattern.Severity.Equals(TEXT("high")) || Pattern.Severity.Equals(TEXT("medium")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Significant pattern: %s (Severity: %s) - %s"), *Pattern.PatternType, *Pattern.Severity, *Pattern.Description);
    }
    else if (ZScores.bOverallAnomaly)
    {
        UE_LOG(LogTemp, Log, TEXT("Anomaly detected: %s (Freq Z: %.2f, Amp Z: %.2f)"), *Pattern.Description, ZScores.FrequencyZ, ZScores.AmplitudeZ);
    }
}

FClusterSummary UBrainwaveAnalysisLibrary::DetectPatternClusters(int32 LookbackMinutes)
{
    FClusterSummary S; S.LookbackMinutes = LookbackMinutes;
    if (GPatternHistory.Num() == 0)
    {
        S.Summary = TEXT("No pattern history available");
        return S;
    }

    const double Cutoff = FDateTime::UtcNow().ToUnixTimestamp() - (LookbackMinutes * 60.0);
    TArray<FPatternAnalysisUE> Recent;
    for (const FPatternAnalysisUE& P : GPatternHistory)
    {
        FDateTime PT; if (!FDateTime::ParseIso8601(*P.TimestampIso, PT)) continue;
        if (PT.ToUnixTimestamp() >= Cutoff) Recent.Add(P);
    }
    S.TotalPatterns = Recent.Num();
    if (Recent.Num() == 0)
    {
        S.Summary = FString::Printf(TEXT("No patterns in last %d minutes"), LookbackMinutes);
        return S;
    }

    TMap<FString, TArray<FPatternAnalysisUE>> Groups;
    for (const FPatternAnalysisUE& P : Recent)
    {
        Groups.FindOrAdd(P.PatternType).Add(P);
    }

    TArray<FPatternCluster> Clusters;
    for (auto& KVP : Groups)
    {
        const TArray<FPatternAnalysisUE>& Arr = KVP.Value;
        if (Arr.Num() >= 3)
        {
            FPatternCluster C;
            C.PatternType = KVP.Key;
            C.Count = Arr.Num();
            // first/last
            C.FirstOccurrenceIso = Arr[0].TimestampIso;
            C.LastOccurrenceIso = Arr.Last().TimestampIso;
            FDateTime First, Last;
            if (FDateTime::ParseIso8601(*C.FirstOccurrenceIso, First) && FDateTime::ParseIso8601(*C.LastOccurrenceIso, Last))
            {
                C.DurationMinutes = (Last - First).GetTotalMinutes();
            }
            // severity distribution
            for (const FPatternAnalysisUE& P2 : Arr)
            {
                C.SeverityDistribution.FindOrAdd(P2.Severity)++;
            }
            // significance
            auto SevWeight = [](const FString& Sev){ if (Sev==TEXT("high")) return 3; if (Sev==TEXT("medium")) return 2; return 1; };
            FString MaxKey = TEXT("low"); int32 MaxW = 1;
            for (const auto& SevKVP : C.SeverityDistribution)
            {
                const int32 W = SevWeight(SevKVP.Key);
                if (W > MaxW) { MaxW = W; MaxKey = SevKVP.Key; }
            }
            C.SignificanceScore = C.Count * MaxW;
            Clusters.Add(C);
        }
    }

    Clusters.Sort([](const FPatternCluster& A, const FPatternCluster& B){ return A.SignificanceScore > B.SignificanceScore; });
    S.Clusters = MoveTemp(Clusters);
    S.Summary = FString::Printf(TEXT("Found %d pattern clusters in last %d minutes"), S.Clusters.Num(), LookbackMinutes);
    return S;
}

FString UBrainwaveAnalysisLibrary::GenerateZScoreReport(double PulseFrequency, double PulseAmplitude)
{
    if (!GbHasBaseline)
    {
        return TEXT("No baseline established. Please call EstablishZBaseline first.");
    }
    const FZScoreResult Z = CalculateZScores(PulseFrequency, PulseAmplitude);
    const FPatternAnalysisUE P = TrackPatternChanges(Z);
    LogBrainwavePattern(PulseFrequency, PulseAmplitude, Z, P);
    const FClusterSummary C = DetectPatternClusters(60);

    FString Report;
    Report += TEXT("\n");
    Report += TEXT("\x2554"); // top-left corner; leave plain text framing minimal
    Report += TEXT(" Z-SCORE BRAINWAVE ANALYSIS \n");
    Report += FString::Printf(TEXT("Frequency: %.2f Hz, Amplitude: %.4f\n"), PulseFrequency, PulseAmplitude);
    Report += FString::Printf(TEXT("Timestamp: %s\n"), *Z.TimestampIso);
    Report += FString::Printf(TEXT("Frequency Z: %.2f, Amplitude Z: %.2f, Overall: %s, Count: %d\n"), Z.FrequencyZ, Z.AmplitudeZ, Z.bOverallAnomaly?TEXT("true"):TEXT("false"), Z.AnomalyCount);
    Report += FString::Printf(TEXT("Pattern: %s | Severity: %s | %s\n"), *P.PatternType, *P.Severity, *P.Description);
    const double AgeMin = NowToMinutesSince(GZBaseline.TimestampIso);
    Report += FString::Printf(TEXT("Baseline Age: %.1f minutes | FreqMean: %.2f Hz | FreqStd: %.2f Hz\n"), AgeMin, GZBaseline.Frequency.Mean, GZBaseline.Frequency.Std);
    if (GZBaseline.bHasAmplitude)
    {
        Report += FString::Printf(TEXT("AmpMean: %.4f | AmpStd: %.4f\n"), GZBaseline.Amplitude.Mean, GZBaseline.Amplitude.Std);
    }
    Report += FString::Printf(TEXT("Clusters (last %d min): %d total patterns, %d clusters\n"), C.LookbackMinutes, C.TotalPatterns, C.Clusters.Num());
    for (int32 i=0; i<FMath::Min(3, C.Clusters.Num()); ++i)
    {
        const FPatternCluster& Cl = C.Clusters[i];
        Report += FString::Printf(TEXT(" - %s (Count: %d)\n"), *Cl.PatternType, Cl.Count);
    }
    return Report;
}

FString UBrainwaveAnalysisLibrary::ExportPatternLog(const FString& Filename)
{
    FString OutFile = Filename;
    if (OutFile.IsEmpty())
    {
        const FString Timestamp = FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S"));
        OutFile = FPaths::Combine(FPaths::ProjectSavedDir(), FString::Printf(TEXT("brainwave_pattern_log_%s.json"), *Timestamp));
    }

    FString Json = TEXT("{\n");
    Json += TEXT("  \"export_timestamp\": \"") + FDateTime::UtcNow().ToIso8601() + TEXT("\",\n");
    Json += TEXT("  \"z_score_threshold\": ") + FString::SanitizeFloat(GZThreshold) + TEXT(",\n");
    Json += TEXT("  \"baseline_stats\": {\n");
    Json += TEXT("    \"frequency\": { \"mean\": ") + FString::SanitizeFloat(GZBaseline.Frequency.Mean) + TEXT(", \"std\": ") + FString::SanitizeFloat(GZBaseline.Frequency.Std) + TEXT(", \"count\": ") + FString::FromInt(GZBaseline.Frequency.Count) + TEXT(" },\n");
    if (GZBaseline.bHasAmplitude)
    {
        Json += TEXT("    \"amplitude\": { \"mean\": ") + FString::SanitizeFloat(GZBaseline.Amplitude.Mean) + TEXT(", \"std\": ") + FString::SanitizeFloat(GZBaseline.Amplitude.Std) + TEXT(", \"count\": ") + FString::FromInt(GZBaseline.Amplitude.Count) + TEXT(" },\n");
    }
    Json += TEXT("    \"timestamp\": \"") + GZBaseline.TimestampIso + TEXT("\"\n  },\n");

    Json += TEXT("  \"pattern_history\": [\n");
    for (int32 i=0;i<GPatternHistory.Num();++i)
    {
        const auto& P = GPatternHistory[i];
        Json += TEXT("    { \"timestamp\": \"") + P.TimestampIso + TEXT("\", \"pattern_type\": \"") + P.PatternType + TEXT("\", \"severity\": \"") + P.Severity + TEXT("\", \"description\": \"") + P.Description.ReplaceCharWithEscapedChar() + TEXT("\" }");
        Json += (i < GPatternHistory.Num()-1) ? TEXT(",\n") : TEXT("\n");
    }
    Json += TEXT("  ],\n");

    Json += TEXT("  \"pattern_log\": [\n");
    for (int32 i=0;i<GPatternLog.Num();++i)
    {
        const auto& E = GPatternLog[i];
        Json += TEXT("    { \"timestamp\": \"") + E.TimestampIso + TEXT("\", \"pulseFrequency\": ") + FString::SanitizeFloat(E.PulseFrequency) + TEXT(", \"pulseAmplitude\": ") + FString::SanitizeFloat(E.PulseAmplitude) + TEXT(" }");
        Json += (i < GPatternLog.Num()-1) ? TEXT(",\n") : TEXT("\n");
    }
    Json += TEXT("  ]\n");
    Json += TEXT("}\n");

    FFileHelper::SaveStringToFile(Json, *OutFile);
    return OutFile;
}

FQuantumCounts UBrainwaveAnalysisLibrary::RunQuantumCounts(int32 Shots, double ProbabilityOneOverride)
{
    FQuantumCounts R;
    R.Shots = Shots;
    R.ProbabilityOne = FMath::Clamp(ProbabilityOneOverride, 0.0, 1.0);
    const int32 Ones = FMath::Clamp(static_cast<int32>(FMath::RoundToDouble(R.ProbabilityOne * double(Shots))), 0, Shots);
    const int32 Zeros = Shots - Ones;
    R.Counts.Add(TEXT("0"), Zeros);
    R.Counts.Add(TEXT("1"), Ones);
    return R;
}

FCognitionRangeResult UBrainwaveAnalysisLibrary::AnalyzeChakraRangeCognition(const FString& ChakraName, double MinFreq, double MaxFreq, int32 NumPoints, int32 Shots)
{
    FCognitionRangeResult R;
    R.Chakra = ChakraName;
    const TArray<double> Freqs = GenerateFrequencyRange(MinFreq, MaxFreq, NumPoints, 1.0);
    R.Batch = BatchAnalyzeFrequencies(Freqs);
    R.Summary = SummarizeBatch(R.Batch);

    // Mean frequency
    if (R.Batch.Frequencies.Num() > 0)
    {
        double Sum = 0.0; for (double f : R.Batch.Frequencies) Sum += f;
        R.MeanFrequency = Sum / double(R.Batch.Frequencies.Num());
    }
    else
    {
        R.MeanFrequency = 0.0;
    }

    // Quantum counts with unbiased H-like distribution (0.5), could be parameterized
    R.Quantum = RunQuantumCounts(Shots, 0.5);
    return R;
}
