#include "BayesianLearningComponent.h"
#include "Engine/World.h"

UBayesianLearningComponent::UBayesianLearningComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBayesianLearningComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initial calculation and log
    CalculatePosterior();
    UE_LOG(LogTemp, Log, TEXT("Class 0 Posterior: %f"), PosteriorClass0);
    UE_LOG(LogTemp, Log, TEXT("Class 1 Posterior: %f"), PosteriorClass1);
}

void UBayesianLearningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Generate new evidence (replace with real data in practice)
    Evidence = FMath::FRandRange(0.f, 1.f);
    UE_LOG(LogTemp, Log, TEXT("New Evidence: %f"), Evidence);

    // Recalculate posterior probabilities
    CalculatePosterior();

    // Output updated posterior probabilities
    UE_LOG(LogTemp, Log, TEXT("Updated Class 0 Posterior: %f"), PosteriorClass0);
    UE_LOG(LogTemp, Log, TEXT("Updated Class 1 Posterior: %f"), PosteriorClass1);

    // Decide which class is more probable
    const bool bClass0MoreProbable = PosteriorClass0 > PosteriorClass1;
    UE_LOG(LogTemp, Log, TEXT("%s"), bClass0MoreProbable ? TEXT("Class 0 is more probable.") : TEXT("Class 1 is more probable."));
}

void UBayesianLearningComponent::CalculatePosterior()
{
    // Marginal likelihood P(E)
    const float MarginalLikelihood = (LikelihoodClass0 * PriorClass0) + (LikelihoodClass1 * PriorClass1);

    if (FMath::IsNearlyZero(MarginalLikelihood))
    {
        PosteriorClass0 = 0.f;
        PosteriorClass1 = 0.f;
        return;
    }

    // Posteriors via Bayes' theorem
    PosteriorClass0 = (LikelihoodClass0 * PriorClass0) / MarginalLikelihood;
    PosteriorClass1 = (LikelihoodClass1 * PriorClass1) / MarginalLikelihood;
}

