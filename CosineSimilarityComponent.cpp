#include "CosineSimilarityComponent.h"
#include "Logging/LogMacros.h"

UCosineSimilarityComponent::UCosineSimilarityComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCosineSimilarityComponent::BeginPlay()
{
    Super::BeginPlay();

    const float Similarity = CalculateCosineSimilarity(VectorA, VectorB);

    if (!FMath::IsNaN(Similarity))
    {
        UE_LOG(LogTemp, Log, TEXT("Cosine Similarity between %s and %s: %.4f"), *VectorA.ToString(), *VectorB.ToString(), Similarity);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Cosine Similarity calculation resulted in NaN. Check for zero-length vectors."));
    }
}

float UCosineSimilarityComponent::CalculateCosineSimilarity(const FVector& A, const FVector& B) const
{
    const float DotProduct = FVector::DotProduct(A, B);
    const float MagnitudeA = A.Size();
    const float MagnitudeB = B.Size();

    if (MagnitudeA <= KINDA_SMALL_NUMBER || MagnitudeB <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogTemp, Warning, TEXT("One or both vectors have zero magnitude. Cannot compute cosine similarity."));
        return NAN;
    }

    return DotProduct / (MagnitudeA * MagnitudeB);
}
