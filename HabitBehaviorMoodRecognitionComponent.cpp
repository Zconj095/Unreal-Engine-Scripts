#include "HabitBehaviorMoodRecognitionComponent.h"
#include "Engine/Engine.h"
#include "Async/Async.h"
#include "Algo/Accumulate.h"

namespace
{
	float ComputeKeywordBonus(const TArray<FString>& Compounds, const TArray<FString>& Keywords)
	{
		int32 Hits = 0;
		for (const FString& Compound : Compounds)
		{
			const FString Lower = Compound.ToLower();
			for (const FString& Keyword : Keywords)
			{
				if (Lower.Contains(Keyword))
				{
					++Hits;
					break;
				}
			}
		}

		return FMath::Min(0.2f, Hits * 0.05f);
	}

	void ComputeArtificialChemistrySignals(const FArtificialChemistrySignature& Signature, float& OutChurchSignal, float& OutHospitalSignal)
	{
		static const TArray<FString> ChurchKeywords = { TEXT("incense"), TEXT("myrrh"), TEXT("resin"), TEXT("frankincense"), TEXT("rose") };
		static const TArray<FString> HospitalKeywords = { TEXT("alcohol"), TEXT("chlorine"), TEXT("antiseptic"), TEXT("iodine"), TEXT("bleach") };

		const float ChurchBonus = ComputeKeywordBonus(Signature.VolatileCompounds, ChurchKeywords);
		const float HospitalBonus = ComputeKeywordBonus(Signature.VolatileCompounds, HospitalKeywords);

		OutChurchSignal = FMath::Clamp(Signature.SacredAromaScore * 0.45f + Signature.RitualResonanceScore * 0.35f + ChurchBonus, 0.0f, 1.0f);
		OutHospitalSignal = FMath::Clamp(Signature.SterileAromaScore * 0.45f + Signature.ClinicalInstrumentationScore * 0.35f + HospitalBonus, 0.0f, 1.0f);
	}

	EArtificialChemistryOrigin ChooseArtificialChemistryOrigin(float ChurchSignal, float HospitalSignal, float Bias, float SignalThreshold)
	{
		if (ChurchSignal < SignalThreshold && HospitalSignal < SignalThreshold)
		{
			return EArtificialChemistryOrigin::Unknown;
		}

		const float Delta = ChurchSignal - HospitalSignal;
		if (Delta > Bias)
		{
			return EArtificialChemistryOrigin::Church;
		}
		if (Delta < -Bias)
		{
			return EArtificialChemistryOrigin::Hospital;
		}
		return EArtificialChemistryOrigin::Blended;
	}
}

UHabitBehaviorMoodRecognitionComponent::UHabitBehaviorMoodRecognitionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHabitBehaviorMoodRecognitionComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("[BehaviorMoodRecognition] Initialized component on %s"), *GetOwner()->GetName());
}

void UHabitBehaviorMoodRecognitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RefreshDendriteLinks();
}

EControlModeRecognition UHabitBehaviorMoodRecognitionComponent::DetermineControlMode(const FBehaviorHabitPattern& Pattern, float AutomaticityThreshold) const
{
	if (Pattern.AutomaticityScore >= AutomaticityThreshold && Pattern.AutomaticityScore >= Pattern.ManagedAttentionScore)
	{
		return EControlModeRecognition::AutomaticControl;
	}

	if (Pattern.ManagedAttentionScore > Pattern.AutomaticityScore && Pattern.ManagedAttentionScore >= AutomaticityThreshold)
	{
		return EControlModeRecognition::ManagedControl;
	}

	if (Pattern.AutomaticityScore > 0.0f && Pattern.ManagedAttentionScore > 0.0f)
	{
		return EControlModeRecognition::HybridControl;
	}

	return EControlModeRecognition::Unknown;
}

EBehaviorHabitCategory UHabitBehaviorMoodRecognitionComponent::DistinguishHabitBehavior(const FBehaviorHabitPattern& Pattern, float Bias) const
{
	const float Difference = Pattern.HabitWeight - Pattern.BehaviorWeight;

	if (Difference > Bias)
	{
		return EBehaviorHabitCategory::Habit;
	}

	if (Difference < -Bias)
	{
		return EBehaviorHabitCategory::Behavior;
	}

	return EBehaviorHabitCategory::Balanced;
}

EEtiquetteMannerismCategory UHabitBehaviorMoodRecognitionComponent::DistinguishEtiquetteMannerism(const FEtiquetteMannerismPair& Pair, float BlendThreshold) const
{
	const float ScoreDelta = Pair.EtiquetteScore - Pair.MannerismScore;

	if (ScoreDelta > BlendThreshold)
	{
		return EEtiquetteMannerismCategory::Etiquette;
	}

	if (ScoreDelta < -BlendThreshold)
	{
		return EEtiquetteMannerismCategory::Mannerism;
	}

	return EEtiquetteMannerismCategory::Hybrid;
}

float UHabitBehaviorMoodRecognitionComponent::SimilarizeEtiquetteAndMannerism(const FEtiquetteMannerismPair& Pair) const
{
	const float BaseScore = (Pair.EtiquetteScore + Pair.MannerismScore) * 0.5f;
	int32 Overlap = 0;

	for (const FString& Qualifier : Pair.EtiquetteQualifiers)
	{
		if (Pair.MannerismQualifiers.Contains(Qualifier))
		{
			++Overlap;
		}
	}

	const float FractionalOverlap = Pair.EtiquetteQualifiers.Num() > 0 ? static_cast<float>(Overlap) / Pair.EtiquetteQualifiers.Num() : 0.0f;
	return FMath::Clamp(BaseScore + FractionalOverlap * 0.25f, 0.0f, 1.0f);
}

float UHabitBehaviorMoodRecognitionComponent::CorrelateMoodWithPattern(const FMoodState& Mood, const FBehaviorHabitPattern& Pattern) const
{
	const float HabitBias = Pattern.HabitWeight - Pattern.BehaviorWeight;
	const float AutoBlend = Pattern.AutomaticityScore * 0.6f + Pattern.ManagedAttentionScore * 0.4f;
	const float MoodSignal = Mood.Valence * HabitBias + Mood.Arousal * AutoBlend + Mood.Focus * Pattern.EtiquetteAlignment;
	return FMath::Clamp(MoodSignal, -1.0f, 1.0f);
}

FString UHabitBehaviorMoodRecognitionComponent::RecognizeOriginFromMood(const FMoodState& Mood, const FBehaviorHabitPattern& Pattern) const
{
	const float Correlation = CorrelateMoodWithPattern(Mood, Pattern);

	if (FMath::Abs(Correlation) < OriginConfidenceThreshold)
	{
		return TEXT("Origin unclear; mood influence is insufficient.");
	}

	const bool bFavorHabits = Correlation > 0.0f;
	const FString ControlModeText = DetermineControlMode(Pattern) == EControlModeRecognition::AutomaticControl ? TEXT("automatic control") : TEXT("managed control");
	return FString::Printf(TEXT("%s mood signature indicates %s as the origin under %s."), *Mood.MoodName.ToString(), bFavorHabits ? TEXT("habit formation") : TEXT("behavior adaptation"), *ControlModeText);
}

bool UHabitBehaviorMoodRecognitionComponent::IsAnimationDrivenByDescriptors(const FMoodState& Mood, const FBehaviorHabitPattern& Pattern, const TArray<FString>& AnimationTags) const
{
	const float OriginCorrelation = CorrelateMoodWithPattern(Mood, Pattern);
	bool bMovementMatch = false;

	for (const FString& Tag : AnimationTags)
	{
		if (Pattern.MovementTags.Contains(Tag))
		{
			bMovementMatch = true;
			break;
		}
	}

	return bMovementMatch && FMath::Abs(OriginCorrelation) >= OriginConfidenceThreshold && Mood.Arousal > 0.3f;
}

FString UHabitBehaviorMoodRecognitionComponent::DistinguishPatternCategories(const FBehaviorHabitPattern& Pattern, const FEtiquetteMannerismPair& Pair, const FMoodState& Mood) const
{
	const FString MoodSummary = FString::Printf(TEXT("Mood:%s(Valence=%.2f,Arousal=%.2f,Focus=%.2f)"), *Mood.MoodName.ToString(), Mood.Valence, Mood.Arousal, Mood.Focus);
	const FString HabitBehavior = FString::Printf(TEXT("HabitWeight=%.2f BehaviorWeight=%.2f"), Pattern.HabitWeight, Pattern.BehaviorWeight);
	const FString EtiquetteMannerism = FString::Printf(TEXT("Etiquette=%.2f Mannerism=%.2f"), Pair.EtiquetteScore, Pair.MannerismScore);
	TArray<FString> Parts;
	Parts.Reserve(3);
	Parts.Add(MoodSummary);
	Parts.Add(HabitBehavior);
	Parts.Add(EtiquetteMannerism);
	return FString::Join(Parts, TEXT(" | "));
}

float UHabitBehaviorMoodRecognitionComponent::PredictMuscleMemoryResponse(const FDendriteMuscleMemoryLink& Link, const FMoodState& Mood) const
{
	const float BaseResponse = Link.SynapticStrength * Mood.Valence * Link.MuscleMemorySensitivity;
	return FMath::Clamp(BaseResponse, -1.0f, 1.0f);
}

void UHabitBehaviorMoodRecognitionComponent::UpdateCurrentMood(const FMoodState& NewMood)
{
	CurrentMood = NewMood;
	UE_LOG(LogTemp, Log, TEXT("[BehaviorMoodRecognition] Mood updated to %s (Valence=%.2f, Arousal=%.2f, Focus=%.2f)"), *NewMood.MoodName.ToString(), NewMood.Valence, NewMood.Arousal, NewMood.Focus);
}

void UHabitBehaviorMoodRecognitionComponent::RefreshDendriteLinks()
{
	for (FDendriteMuscleMemoryLink& Link : DendriteLinks)
	{
		Link.MoodAnchor = CurrentMood;
	}
}

EAutonomicControlAction UHabitBehaviorMoodRecognitionComponent::RecognizeAutonomicControlAction(const FBehaviorHabitPattern& Pattern, float AutonomicResponseScore, float ControlSensitivity) const
{
	const float AutoVsManaged = Pattern.AutomaticityScore - Pattern.ManagedAttentionScore;
	const bool bStrongAutonomic = AutonomicResponseScore >= ControlSensitivity;
	const bool bStrongAutomaticBehavior = Pattern.AutomaticityScore >= ControlSensitivity;

	if (bStrongAutonomic && bStrongAutomaticBehavior)
	{
		return EAutonomicControlAction::Use; // letting autonomic/automatic drive while leveraging it
	}

	if (bStrongAutonomic && AutoVsManaged < -0.1f)
	{
		return EAutonomicControlAction::Manage; // autonomic surge but managed attention dominates
	}

	if (!bStrongAutonomic && bStrongAutomaticBehavior)
	{
		return EAutonomicControlAction::Control; // automatic habit present but autonomic response subdued
	}

	if (!bStrongAutonomic && AutoVsManaged > 0.25f)
	{
		return EAutonomicControlAction::Manipulate; // bias toward automaticity being intentionally steered
	}

	return EAutonomicControlAction::Unclear;
}

FString UHabitBehaviorMoodRecognitionComponent::EvaluateAffectState(const FSocialAffectState& Affect) const
{
	const float Warmth = (Affect.Affection + Affect.Friendship + Affect.Happiness) / 3.0f;
	const float Integrity = Affect.IndividualValues * 0.5f + Affect.EffortValue * 0.5f;
	const FString WarmthText = Warmth > 0.66f ? TEXT("High warmth") : Warmth > 0.33f ? TEXT("Moderate warmth") : TEXT("Low warmth");
	const FString IntegrityText = Integrity > 0.66f ? TEXT("Aligned values/effort") : Integrity > 0.33f ? TEXT("Developing values/effort") : TEXT("Weak values/effort");
	return FString::Printf(TEXT("%s; %s."), *WarmthText, *IntegrityText);
}

ESpaceFormType UHabitBehaviorMoodRecognitionComponent::RecognizeSpaceForm(const FString& Term) const
{
	const FString Lower = Term.ToLower();
	if (Lower.Contains(TEXT("spatial")))
	{
		return ESpaceFormType::Spatial;
	}
	if (Lower.Contains(TEXT("spatio")))
	{
		return ESpaceFormType::Spatio;
	}
	if (Lower.Contains(TEXT("spatal")))
	{
		return ESpaceFormType::Spatal;
	}
	return ESpaceFormType::Unknown;
}

float UHabitBehaviorMoodRecognitionComponent::RecognizeRhythmFromSequences(const TArray<float>& MelodyIntervals, const TArray<float>& HarmonyIntervals) const
{
	const float MelodyMean = MelodyIntervals.Num() > 0 ? Algo::Accumulate(MelodyIntervals, 0.0f) / MelodyIntervals.Num() : 0.0f;
	const float HarmonyMean = HarmonyIntervals.Num() > 0 ? Algo::Accumulate(HarmonyIntervals, 0.0f) / HarmonyIntervals.Num() : 0.0f;
	const float RhythmBlend = (MelodyMean + HarmonyMean) * 0.5f;
	return RhythmBlend;
}

FString UHabitBehaviorMoodRecognitionComponent::RecognizeSpaceRhythm(ESpaceFormType SpaceForm, const TArray<float>& RhythmPattern) const
{
	const float Mean = RhythmPattern.Num() > 0 ? Algo::Accumulate(RhythmPattern, 0.0f) / RhythmPattern.Num() : 0.0f;
	const TCHAR* SpaceLabel =
		SpaceForm == ESpaceFormType::Spatial ? TEXT("Spatial") :
		SpaceForm == ESpaceFormType::Spatio ? TEXT("Spatio") :
		SpaceForm == ESpaceFormType::Spatal ? TEXT("Spatal") : TEXT("Unknown");

	const FString RhythmLabel = Mean < 0.4f ? TEXT("calm rhythm") : Mean < 0.7f ? TEXT("steady rhythm") : TEXT("intense rhythm");
	return FString::Printf(TEXT("%s space with %s (mean=%.2f)"), SpaceLabel, *RhythmLabel, Mean);
}

void UHabitBehaviorMoodRecognitionComponent::StartAsyncMelodyVsLullabyCheck(const TArray<float>& NoteIntervals, float LullabyThreshold)
{
	const TArray<float> CaptureIntervals = NoteIntervals;
	Async(EAsyncExecution::ThreadPool, [this, CaptureIntervals, LullabyThreshold]()
	{
		const float MeanInterval = CaptureIntervals.Num() > 0 ? Algo::Accumulate(CaptureIntervals, 0.0f) / CaptureIntervals.Num() : 0.0f;
		const bool bIsLullaby = MeanInterval >= LullabyThreshold;
		const FString Tag = bIsLullaby ? TEXT("Lullaby") : TEXT("Melody");

		AsyncTask(ENamedThreads::GameThread, [this, bIsLullaby, Tag]()
		{
			bLastAsyncCheckWasLullaby = bIsLullaby;
			LastAsyncMelodyTag = Tag;
			UE_LOG(LogTemp, Log, TEXT("[BehaviorMoodRecognition] Async melody check tagged as %s"), *Tag);
		});
	});
}

bool UHabitBehaviorMoodRecognitionComponent::DistinguishHarmonySync(const TArray<float>& HarmonyIntervals, float ConsonanceThreshold) const
{
	if (HarmonyIntervals.Num() == 0)
	{
		return false;
	}

	float Variance = 0.0f;
	float Mean = 0.0f;
	for (float V : HarmonyIntervals)
	{
		Mean += V;
	}
	Mean /= HarmonyIntervals.Num();

	for (float V : HarmonyIntervals)
	{
		const float Delta = V - Mean;
		Variance += Delta * Delta;
	}
	Variance /= HarmonyIntervals.Num();

	const float Consonance = 1.0f - FMath::Clamp(Variance, 0.0f, 1.0f);
	return Consonance >= ConsonanceThreshold;
}

EVoidValueOrigin UHabitBehaviorMoodRecognitionComponent::RecognizeVoidValueOrigin(float NothingScore, float EmptyScore, float Bias) const
{
	const float Delta = NothingScore - EmptyScore;
	if (Delta > Bias)
	{
		return EVoidValueOrigin::MadeFromNothing;
	}
	if (Delta < -Bias)
	{
		return EVoidValueOrigin::MadeFromEmpty;
	}
	return EVoidValueOrigin::Undefined;
}

ECleanlinessCategory UHabitBehaviorMoodRecognitionComponent::ClassifyCleanliness(float CleanlinessScore, float DirtinessScore, float Threshold) const
{
	const float Delta = CleanlinessScore - DirtinessScore;
	if (Delta > Threshold)
	{
		return ECleanlinessCategory::CleanOrNeat;
	}
	if (Delta < -Threshold)
	{
		return ECleanlinessCategory::DirtyOrFilthy;
	}
	return ECleanlinessCategory::Unclear;
}

FString UHabitBehaviorMoodRecognitionComponent::RecognizeRhythmChangeWithUnits(float TemperatureF, float WeightLbs, const TArray<float>& RhythmPattern, ESpaceFormType SpaceForm) const
{
	if (RhythmPattern.Num() == 0)
	{
		return TEXT("No rhythm data provided.");
	}

	float MinVal = RhythmPattern[0];
	float MaxVal = RhythmPattern[0];
	for (float V : RhythmPattern)
	{
		MinVal = FMath::Min(MinVal, V);
		MaxVal = FMath::Max(MaxVal, V);
	}

	const float ChangeMagnitude = MaxVal - MinVal;
	const FString RhythmType = ChangeMagnitude < 0.2f ? TEXT("steady rhythm") : ChangeMagnitude < 0.6f ? TEXT("dynamic rhythm") : TEXT("volatile rhythm");
	const TCHAR* SpaceLabel =
		SpaceForm == ESpaceFormType::Spatial ? TEXT("Spatial") :
		SpaceForm == ESpaceFormType::Spatio ? TEXT("Spatio") :
		SpaceForm == ESpaceFormType::Spatal ? TEXT("Spatal") : TEXT("Unknown");

	return FString::Printf(TEXT("%s space | Rhythm=%s (Δ=%.2f) | Temp=%.1f°F | Weight=%.1f lbs"),
		SpaceLabel, *RhythmType, ChangeMagnitude, TemperatureF, WeightLbs);
}

FString UHabitBehaviorMoodRecognitionComponent::RecognizePlacementVsSchedule(bool bHasBeginningPlace, bool bHasPlacement, bool bHasSchedule) const
{
	if (bHasBeginningPlace && !bHasPlacement && !bHasSchedule)
	{
		return TEXT("Beginning place identified; placement and schedule pending.");
	}
	if (bHasPlacement && !bHasSchedule)
	{
		return TEXT("Placement locked; schedule not set.");
	}
	if (bHasPlacement && bHasSchedule)
	{
		return TEXT("Placement fixed and schedule confirmed.");
	}
	if (!bHasBeginningPlace && (bHasPlacement || bHasSchedule))
	{
		return TEXT("Placement/schedule present but origin unclear.");
	}
	return TEXT("No placement or schedule information.");
}

FString UHabitBehaviorMoodRecognitionComponent::RecognizeBeginStartOnsetOffset(float BeginTime, float StartTime, float OnsetTime, float OffsetTime, const FString& SpaceLabel) const
{
	const FString BeginText = FString::Printf(TEXT("Begin=%.2f"), BeginTime);
	const FString StartText = FString::Printf(TEXT("Start=%.2f"), StartTime);
	const FString OnsetText = FString::Printf(TEXT("Onset=%.2f"), OnsetTime);
	const FString OffsetText = FString::Printf(TEXT("Offset=%.2f"), OffsetTime);
	return FString::Printf(TEXT("%s | %s | %s | %s | Space=%s"), *BeginText, *StartText, *OnsetText, *OffsetText, *SpaceLabel);
}

ECoordinateSystemKind UHabitBehaviorMoodRecognitionComponent::RecognizeCoordinateSystem(bool bHasMetric, bool bUsesAxisNamesXYZ, bool bIsAffineNormalized) const
{
	if (bHasMetric && bUsesAxisNamesXYZ)
	{
		return ECoordinateSystemKind::Cartesian;
	}
	if (bHasMetric && !bUsesAxisNamesXYZ)
	{
		return ECoordinateSystemKind::Euclidean;
	}
	if (!bHasMetric && bUsesAxisNamesXYZ && bIsAffineNormalized)
	{
		return ECoordinateSystemKind::Cartesian;
	}
	return ECoordinateSystemKind::CoordUnknown;
}

EZValueCategory UHabitBehaviorMoodRecognitionComponent::RecognizeZScoreOrdinalCardinal(bool bIsMeanCentered, bool bIsStandardized, bool bIsRanked, bool bIsCountBased) const
{
	if (bIsMeanCentered && bIsStandardized)
	{
		return EZValueCategory::ZScore;
	}
	if (bIsRanked)
	{
		return EZValueCategory::Ordinal;
	}
	if (bIsCountBased)
	{
		return EZValueCategory::Cardinal;
	}
	return EZValueCategory::ZUnknown;
}

EHandednessType UHabitBehaviorMoodRecognitionComponent::RecognizeHandedness(float BasisDeterminant, bool bCrossProductMatchesRightHandRule, float Epsilon) const
{
	if (FMath::Abs(BasisDeterminant) <= Epsilon)
	{
		return EHandednessType::HandedUnknown;
	}

	if (BasisDeterminant > Epsilon && bCrossProductMatchesRightHandRule)
	{
		return EHandednessType::RightHanded;
	}

	if (BasisDeterminant < -Epsilon && !bCrossProductMatchesRightHandRule)
	{
		return EHandednessType::LeftHanded;
	}

	return EHandednessType::HandedUnknown;
}

EChangeProcessCategory UHabitBehaviorMoodRecognitionComponent::RecognizeChangeProcess(float PermutationScore, float TransformScore, float MorphScore, float MutateScore, float Bias) const
{
	const float Scores[4] = { PermutationScore, TransformScore, MorphScore, MutateScore };
	int32 MaxIdx = 0;
	for (int32 i = 1; i < 4; ++i)
	{
		if (Scores[i] > Scores[MaxIdx])
		{
			MaxIdx = i;
		}
	}

	const float MaxVal = Scores[MaxIdx];
	auto IsDominant = [&](float Value) { return (MaxVal - Value) <= Bias; };

	switch (MaxIdx)
	{
	case 0: return IsDominant(PermutationScore) ? EChangeProcessCategory::Permutate : EChangeProcessCategory::ChangeUnknown;
	case 1: return IsDominant(TransformScore) ? EChangeProcessCategory::Transform : EChangeProcessCategory::ChangeUnknown;
	case 2: return IsDominant(MorphScore) ? EChangeProcessCategory::Morph : EChangeProcessCategory::ChangeUnknown;
	case 3: return IsDominant(MutateScore) ? EChangeProcessCategory::Mutate : EChangeProcessCategory::ChangeUnknown;
	default: return EChangeProcessCategory::ChangeUnknown;
	}
}

EPolyChangeCategory UHabitBehaviorMoodRecognitionComponent::RecognizePolyChange(float PolymerizationScore, float PolymorphismScore, float MetamorphosisScore, float TransmutationScore, float Bias) const
{
	const float Scores[4] = { PolymerizationScore, PolymorphismScore, MetamorphosisScore, TransmutationScore };
	int32 MaxIdx = 0;
	for (int32 i = 1; i < 4; ++i)
	{
		if (Scores[i] > Scores[MaxIdx])
		{
			MaxIdx = i;
		}
	}

	const float MaxVal = Scores[MaxIdx];
	auto IsDominant = [&](float Value) { return (MaxVal - Value) <= Bias; };

	switch (MaxIdx)
	{
	case 0: return IsDominant(PolymerizationScore) ? EPolyChangeCategory::Polymerization : EPolyChangeCategory::PolyUnknown;
	case 1: return IsDominant(PolymorphismScore) ? EPolyChangeCategory::Polymorphism : EPolyChangeCategory::PolyUnknown;
	case 2: return IsDominant(MetamorphosisScore) ? EPolyChangeCategory::Metamorphosis : EPolyChangeCategory::PolyUnknown;
	case 3: return IsDominant(TransmutationScore) ? EPolyChangeCategory::Transmutation : EPolyChangeCategory::PolyUnknown;
	default: return EPolyChangeCategory::PolyUnknown;
	}
}

EGeometryCategory UHabitBehaviorMoodRecognitionComponent::RecognizeGeometryType(float BasicScore, float AbstractScore, float SacredScore, float Bias) const
{
	const float Scores[3] = { BasicScore, AbstractScore, SacredScore };
	int32 MaxIdx = 0;
	for (int32 i = 1; i < 3; ++i)
	{
		if (Scores[i] > Scores[MaxIdx])
		{
			MaxIdx = i;
		}
	}

	const float MaxVal = Scores[MaxIdx];
	auto IsDominant = [&](float Value) { return (MaxVal - Value) <= Bias; };

	switch (MaxIdx)
	{
	case 0: return IsDominant(BasicScore) ? EGeometryCategory::BasicGeometry : EGeometryCategory::GeometryUnknown;
	case 1: return IsDominant(AbstractScore) ? EGeometryCategory::AbstractGeometry : EGeometryCategory::GeometryUnknown;
	case 2: return IsDominant(SacredScore) ? EGeometryCategory::SacredGeometry : EGeometryCategory::GeometryUnknown;
	default: return EGeometryCategory::GeometryUnknown;
	}
}

EEncodingCategory UHabitBehaviorMoodRecognitionComponent::RecognizeEncodingCategory(const FString& Term) const
{
	const FString Lower = Term.ToLower();

	if (Lower.Contains(TEXT("qubit")) || Lower.Contains(TEXT("quantum")))
	{
		return EEncodingCategory::Qubit;
	}
	if (Lower.Contains(TEXT("ternary")) || Lower.Contains(TEXT("trit")))
	{
		return EEncodingCategory::Ternary;
	}
	if (Lower.Contains(TEXT("binary")) || Lower.Contains(TEXT("bit")))
	{
		return EEncodingCategory::Binary;
	}
	if (Lower.Contains(TEXT("hexadecimal")) || Lower.Contains(TEXT("hex")) || Lower.Contains(TEXT("base16")))
	{
		return EEncodingCategory::Hexadecimal;
	}
	return EEncodingCategory::EncodingUnknown;
}

EMatterClassification UHabitBehaviorMoodRecognitionComponent::RecognizeMatterClassification(bool bIsFundamental, bool bHasNetCharge, int32 AtomCount) const
{
	if (bIsFundamental && !bHasNetCharge && AtomCount <= 1)
	{
		return EMatterClassification::Particle;
	}
	if (bHasNetCharge)
	{
		return EMatterClassification::Ion;
	}
	if (AtomCount > 1 && !bHasNetCharge)
	{
		return EMatterClassification::Molecule;
	}
	return EMatterClassification::MatterUnknown;
}

EArtificialChemistryOrigin UHabitBehaviorMoodRecognitionComponent::RecognizeArtificialChemistryOrigin(const FArtificialChemistrySignature& Signature, float Bias, float SignalThreshold) const
{
	float ChurchSignal = 0.0f;
	float HospitalSignal = 0.0f;
	ComputeArtificialChemistrySignals(Signature, ChurchSignal, HospitalSignal);
	return ChooseArtificialChemistryOrigin(ChurchSignal, HospitalSignal, Bias, SignalThreshold);
}

FString UHabitBehaviorMoodRecognitionComponent::DescribeArtificialChemistryOrigin(const FArtificialChemistrySignature& Signature, float Bias, float SignalThreshold) const
{
	float ChurchSignal = 0.0f;
	float HospitalSignal = 0.0f;
	ComputeArtificialChemistrySignals(Signature, ChurchSignal, HospitalSignal);

	const EArtificialChemistryOrigin Origin = ChooseArtificialChemistryOrigin(ChurchSignal, HospitalSignal, Bias, SignalThreshold);
	const TCHAR* OriginLabel =
		Origin == EArtificialChemistryOrigin::Church ? TEXT("Church") :
		Origin == EArtificialChemistryOrigin::Hospital ? TEXT("Hospital") :
		Origin == EArtificialChemistryOrigin::Blended ? TEXT("Blended") : TEXT("Unknown");

	return FString::Printf(TEXT("Artificial chemistry origin=%s | ChurchSignal=%.2f HospitalSignal=%.2f Bias=%.2f Threshold=%.2f"),
		OriginLabel, ChurchSignal, HospitalSignal, Bias, SignalThreshold);
}
