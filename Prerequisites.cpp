#include "IndependentThoughtProcessor.h"
#include "ClassificationManager.h"
#include "Math/UnrealMathUtility.h"

void UIndependentThoughtProcessor::SetEligibilityProfile(const FEligibilityProfile& InProfile)
{
    Profile = InProfile;
}

bool UIndependentThoughtProcessor::ValidateInternal(FString& OutWhyNot) const
{
    TArray<FString> Issues;

    if (Profile.Age < 18) Issues.Add(TEXT("Must be 18+"));
    if (!Profile.Personality.Equals(TEXT("INFJ-T"), ESearchCase::IgnoreCase)) Issues.Add(TEXT("Personality must be INFJ-T"));
    if (!Profile.Zodiac.Equals(TEXT("Pisces"), ESearchCase::IgnoreCase)) Issues.Add(TEXT("Zodiac must be Pisces"));

    // Intent and understanding requirements
    if (!Profile.bForHealthyLifestyle) Issues.Add(TEXT("Requires improving healthy lifestyles"));
    if (!Profile.bImproveLives) Issues.Add(TEXT("Must be used to improve lives"));
    if (!Profile.bGoodIntentions) Issues.Add(TEXT("Requires good intentions"));
    if (!Profile.bUnderstandsEnergyChi) Issues.Add(TEXT("Requires understanding of energy and chi"));
    if (!Profile.bMaintainsCleanSettings) Issues.Add(TEXT("Must maintain clean settings"));
    if (!Profile.bUnderstandsDesignAndDev) Issues.Add(TEXT("Requires understanding of designer and development features"));

    if (!Profile.bCompassion) Issues.Add(TEXT("Requires compassion"));
    if (!Profile.bCreativity) Issues.Add(TEXT("Requires creativity"));
    if (!Profile.bEmpathy) Issues.Add(TEXT("Requires empathy"));
    if (!Profile.bHeightenedSensitivity) Issues.Add(TEXT("Requires heightened sensitivity"));
    if (!Profile.bEmpathyAndSensoryCombined) Issues.Add(TEXT("Requires empathy and sensory combined"));
    if (!Profile.bInnerPeace) Issues.Add(TEXT("Requires forming inner peace"));
    if (!Profile.bSelfLimitsBoundaries) Issues.Add(TEXT("Requires forming limits to improve boundaries"));
    if (!Profile.bEmpathyControl) Issues.Add(TEXT("Requires good control of empathy levels"));
    if (!Profile.bImagination) Issues.Add(TEXT("Requires imagination"));
    if (!Profile.bImproveMindBodySoulSpirit) Issues.Add(TEXT("Requires improving mind, body, soul, and spirit"));
    if (!Profile.bInternalExternalFunctions) Issues.Add(TEXT("Requires internal and external functions"));
    if (!Profile.bInternalHappiness) Issues.Add(TEXT("Requires internal happiness"));
    if (!Profile.bIntuitionAndPrayer) Issues.Add(TEXT("Requires intuition and prayer combined"));
    if (!Profile.bLimitedConnection) Issues.Add(TEXT("Requires limited connection"));
    if (!Profile.bSpiritualAndScientificUnderstanding) Issues.Add(TEXT("Requires spiritual and scientific understanding"));
    if (!Profile.bStrongFaithExternalHopeWhenPraying) Issues.Add(TEXT("Requires strong faith and external hope when forming prayer"));
    if (!Profile.bStrongFaithsAndBeliefs) Issues.Add(TEXT("Requires strong faiths and beliefs"));
    if (!Profile.bSequencePLPL) Issues.Add(TEXT("Requires sequence Perspective + Logical + Perspective + Logical"));

    if (Issues.Num() > 0)
    {
        OutWhyNot = FString::Join(Issues, TEXT("; "));
        return false;
    }
    OutWhyNot.Reset();
    return true;
}

bool UIndependentThoughtProcessor::IsEligible(FString& OutWhyNot) const
{
    return ValidateInternal(OutWhyNot);
}

void UIndependentThoughtProcessor::ExpandClusters(const TArray<FDoubleArray>& BaseClusters, int32 NumNewClusters)
{
    FString WhyNot;
    if (!ValidateInternal(WhyNot))
    {
        UE_LOG(LogTemp, Warning, TEXT("Eligibility check failed: %s"), *WhyNot);
        return;
    }

    // Generate random perturbations to existing centroids
    TArray<FDoubleArray> NewCentroids;
    for (const FDoubleArray& Cluster : BaseClusters)
    {
        const int32 Dim = Cluster.Values.Num();
        for (int32 i = 0; i < NumNewClusters; ++i)
        {
            FDoubleArray NewC;
            NewC.Values.SetNumUninitialized(Dim);
            for (int32 j = 0; j < Dim; ++j)
            {
                const double Noise = FMath::FRandRange(-0.1, 0.1);
                NewC.Values[j] = Cluster.Values[j] + Noise;
            }
            NewCentroids.Add(MoveTemp(NewC));
        }
    }

    // Re-cluster with expanded centroids (append to memory)
    MemoryEmbeddings.Append(NewCentroids);

    if (KMeansManager == nullptr)
    {
        KMeansManager = NewObject<UClassificationManager>(this);
    }

    // Cluster memory embeddings into NumNewClusters groups, mirroring the Unity example
    KMeansManager->InitializeKMeans(NumNewClusters);
    KMeansManager->TrainKMeans(MemoryEmbeddings /*FeatureSet*/);

    UE_LOG(LogTemp, Log, TEXT("Expanded into %d clusters."), NumNewClusters);
}

