#include "InvocationLexiconLibrary.h"

static double Clamp01(double Value)
{
    return FMath::Clamp(Value, 0.0, 1.0);
}

static double Sigmoid(double X)
{
    return 1.0 / (1.0 + FMath::Exp(-X));
}

static double HarmonicMean3(double A, double B, double C)
{
    const double Eps = 1e-6;
    A = FMath::Max(A, Eps);
    B = FMath::Max(B, Eps);
    C = FMath::Max(C, Eps);
    return 3.0 / (1.0 / A + 1.0 / B + 1.0 / C);
}

FString UInvocationLexiconLibrary::ToLabel(EInvocationTerm Term)
{
    switch (Term)
    {
    case EInvocationTerm::PositiveAffirmation: return TEXT("Positive Affirmation");
    case EInvocationTerm::Enchant:             return TEXT("Enchant");
    case EInvocationTerm::ImpressUpon:         return TEXT("Impress Upon");
    case EInvocationTerm::ImbueTowards:        return TEXT("Imbue Towards");
    case EInvocationTerm::InterconnectWith:    return TEXT("Interconnect With");
    case EInvocationTerm::Enchantment:         return TEXT("Enchantment");
    case EInvocationTerm::Spell:               return TEXT("Spell");
    case EInvocationTerm::Spellbind:           return TEXT("Spellbind");
    case EInvocationTerm::MeditateUpon:        return TEXT("Meditate Upon");
    case EInvocationTerm::Ensnare:             return TEXT("Ensnare");
    case EInvocationTerm::Interweave:          return TEXT("Interweave");
    case EInvocationTerm::Prayer:              return TEXT("Prayer");
    case EInvocationTerm::Devotion:            return TEXT("Devotion");
    case EInvocationTerm::Divinity:            return TEXT("Divinity");
    case EInvocationTerm::Deviation:           return TEXT("Deviation");
    case EInvocationTerm::Truths:              return TEXT("Truths");
    }
    return FString();
}

FString UInvocationLexiconLibrary::DefinitionOf(EInvocationTerm Term)
{
    switch (Term)
    {
    case EInvocationTerm::PositiveAffirmation:
        return TEXT("A series of confirmations to set into action for something to come into existence using beliefs and or prayers connected to a higher entity within class and or status and or existence.");
    case EInvocationTerm::Enchant:
        return TEXT("Set a specific intent with beliefs attached to it using linguistic capabilities using language and or literature and or symbolics and or with numerical calculations.");
    case EInvocationTerm::ImpressUpon:
        return TEXT("To attach and embed something formed from belief and or description to an entity.");
    case EInvocationTerm::ImbueTowards:
        return TEXT("To embed and directly integrate a belief and the way that it is formed towards and into an entity.");
    case EInvocationTerm::InterconnectWith:
        return TEXT("Form connections with that connections to other interconnections.");
    case EInvocationTerm::Enchantment:
        return TEXT("A binding and or something imbued upon that interconnects many beliefs into something to enchant with.");
    case EInvocationTerm::Spell:
        return TEXT("An action formed by using impressed meaning and or descriptions bound by rules and behaviors from grammars and spellings of words and descriptions from language and or numbers.");
    case EInvocationTerm::Spellbind:
        return TEXT("A spell that is bound into an object and or many other spells and interconnected between multiple dimensions and or multiple origin locations.");
    case EInvocationTerm::MeditateUpon:
        return TEXT("To sit and absorb and collect information and rapidly process it as a way to make decisions more precisely.");
    case EInvocationTerm::Ensnare:
        return TEXT("To intertwine and bind towards.");
    case EInvocationTerm::Interweave:
        return TEXT("To entangle and ensnare multiple bindings together and interconnect them into many relations and connections.");
    case EInvocationTerm::Prayer:
        return TEXT("To send a request to a entity that is believed to exist and have that request granted through the capability of belief and request.");
    case EInvocationTerm::Devotion:
        return TEXT("The connection of dedication and disciplines to a entity.");
    case EInvocationTerm::Divinity:
        return TEXT("The connection of unity and harmony towards the capability of a entity and devotion of that entity.");
    case EInvocationTerm::Deviation:
        return TEXT("The permutations that form from deviated connections through language and or numbers from the number of mutations and transformations that happen from divinity and or devotion and or prayer and connects to a entity believed to exist.");
    case EInvocationTerm::Truths:
        return TEXT("A series of facts and or bias and or beliefs that connect together into a series of disciplines conditions and or compatible measurements.");
    }
    return FString();
}

static double ComputeAffirmationPotential(const FInvocationInputs& In)
{
    const double Belief = Clamp01(In.BeliefStrength);
    const double Prayer = Clamp01(In.PrayerCoherence);
    const double Intent = Clamp01(In.IntentClarity);
    const double Deviation = Clamp01(In.DeviationEntropy);
    return Sigmoid(2.0 * Belief + 1.2 * Prayer + 1.5 * Intent - 1.1 * Deviation);
}

static double ComputeEnchantIntent(const FInvocationInputs& In)
{
    const double Linguistic = Clamp01(In.LinguisticPrecision);
    const double Symbolic = Clamp01(In.SymbolicResonance);
    const double Numeric = Clamp01(In.NumericAnchor);
    return FMath::Sqrt(Linguistic * Symbolic) * (1.0 + 0.5 * Numeric);
}

static double ComputeImpressStrength(const FInvocationInputs& In)
{
    const double Embed = Clamp01(In.EmbeddingDepth);
    const double Belief = Clamp01(In.BeliefStrength);
    const double Intent = Clamp01(In.IntentClarity);
    return Embed * (0.6 + 0.4 * Belief) * (0.6 + 0.4 * Intent);
}

static double ComputeImbueStrength(const FInvocationInputs& In, double ImpressStrength)
{
    const double Focus = Clamp01(In.DirectionalFocus);
    return ImpressStrength * (0.5 + 0.5 * Focus);
}

static double ComputeInterconnectStrength(const FInvocationInputs& In)
{
    const double Density = Clamp01(In.InterconnectionDensity);
    return 1.0 - FMath::Exp(-2.2 * Density);
}

static double ComputeSpellAction(const FInvocationInputs& In)
{
    const double Grammar = Clamp01(In.GrammarConformance);
    const double Linguistic = Clamp01(In.LinguisticPrecision);
    const double Numeric = Clamp01(In.NumericAnchor);
    return FMath::Pow(Grammar * Linguistic, 0.75) * (1.0 + 0.35 * Numeric);
}

static double ComputePrayerSignal(const FInvocationInputs& In)
{
    const double Belief = Clamp01(In.BeliefStrength);
    const double Prayer = Clamp01(In.PrayerCoherence);
    const double Devotion = Clamp01(In.DevotionStability);
    return FMath::Sqrt(Belief * Prayer) * (0.5 + 0.5 * Devotion);
}

static double ComputeMeditationClarity(const FInvocationInputs& In)
{
    const double Meditate = Clamp01(In.MeditationFocus);
    const double Truth = Clamp01(In.TruthConsistency);
    return FMath::Sqrt(Meditate * Truth);
}

static double ComputeDevotionBond(const FInvocationInputs& In, double PrayerSignal, double MeditationClarity)
{
    const double Devotion = Clamp01(In.DevotionStability);
    const double Core = FMath::Pow(Devotion * MeditationClarity, 0.5);
    return Core * (0.6 + 0.4 * PrayerSignal);
}

static double ComputeDivinityUnity(const FInvocationInputs& In, double DevotionBond)
{
    const double Alignment = Clamp01(In.DivinityAlignment);
    return FMath::Pow(Alignment * DevotionBond, 0.5);
}

static double ComputeDeviationIndex(const FInvocationInputs& In)
{
    const double Deviation = Clamp01(In.DeviationEntropy);
    const double Truth = Clamp01(In.TruthConsistency);
    return Deviation * (1.0 + 0.5 * (1.0 - Truth));
}

static double ComputeTruthCoherence(const FInvocationInputs& In)
{
    const double Truth = Clamp01(In.TruthConsistency);
    const double Belief = Clamp01(In.BeliefStrength);
    return FMath::Pow(Truth * (0.5 + 0.5 * Belief), 0.75);
}

FInvocationMetrics UInvocationLexiconLibrary::ComputeMetrics(const FInvocationInputs& In)
{
    FInvocationMetrics Out;
    Out.AffirmationPotential = ComputeAffirmationPotential(In);
    Out.EnchantIntent = ComputeEnchantIntent(In);
    Out.ImpressStrength = ComputeImpressStrength(In);
    Out.ImbueStrength = ComputeImbueStrength(In, Out.ImpressStrength);
    Out.InterconnectStrength = ComputeInterconnectStrength(In);

    Out.EnchantmentBinding = HarmonicMean3(Out.AffirmationPotential, Out.EnchantIntent, Out.InterconnectStrength);
    Out.SpellAction = ComputeSpellAction(In);

    const double Dimensional = Clamp01(In.DimensionalStability);
    Out.SpellbindAnchor = Out.SpellAction * (0.5 + 0.5 * Dimensional) * (0.4 + 0.6 * Out.InterconnectStrength);

    Out.MeditationClarity = ComputeMeditationClarity(In);
    Out.EnsnareBind = Out.InterconnectStrength * (0.5 + 0.5 * Clamp01(In.EmbeddingDepth));
    Out.InterweaveCoupling = FMath::Sqrt(Out.EnsnareBind * Out.InterconnectStrength);

    Out.PrayerSignal = ComputePrayerSignal(In);
    Out.DevotionBond = ComputeDevotionBond(In, Out.PrayerSignal, Out.MeditationClarity);
    Out.DivinityUnity = ComputeDivinityUnity(In, Out.DevotionBond);

    Out.DeviationIndex = ComputeDeviationIndex(In);
    Out.TruthCoherence = ComputeTruthCoherence(In);

    Out.ManifestationProbability = ComputeManifestationProbability(In);
    Out.Summary = FString::Printf(TEXT("Aff=%.3f, Enchant=%.3f, Spell=%.3f, Bind=%.3f, Pray=%.3f, Deviate=%.3f, Truth=%.3f, P=%.3f"),
        Out.AffirmationPotential,
        Out.EnchantmentBinding,
        Out.SpellAction,
        Out.SpellbindAnchor,
        Out.PrayerSignal,
        Out.DeviationIndex,
        Out.TruthCoherence,
        Out.ManifestationProbability);
    return Out;
}

double UInvocationLexiconLibrary::ComputeManifestationProbability(const FInvocationInputs& In)
{
    const double Affirmation = ComputeAffirmationPotential(In);
    const double Enchant = ComputeEnchantIntent(In);
    const double Interconnect = ComputeInterconnectStrength(In);
    const double Binding = HarmonicMean3(Affirmation, Enchant, Interconnect);
    const double Spellbind = ComputeSpellAction(In) * (0.5 + 0.5 * Clamp01(In.DimensionalStability));
    const double PrayerSignal = ComputePrayerSignal(In);
    const double Truth = ComputeTruthCoherence(In);
    const double Deviation = ComputeDeviationIndex(In);

    const double Energy = 2.4 * Binding + 2.1 * Spellbind + 1.6 * PrayerSignal + 1.4 * Truth - 2.3 * Deviation;
    return Sigmoid(Energy);
}
