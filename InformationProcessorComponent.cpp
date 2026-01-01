#include "InformationProcessorComponent.h"

#include "Math/UnrealMathUtility.h"

UInformationProcessorComponent::UInformationProcessorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInformationProcessorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!bRunDemoOnBeginPlay)
    {
        return;
    }

    // Demo: Retention
    const float Retention = CalculateRetention(RetentionWeights, RetentionAlpha, RetentionTime);
    UE_LOG(LogTemp, Log, TEXT("Retention Value: %f"), Retention);

    // Demo: Average Similarity
    TArray<TArray<float>> Groups;
    Groups.Add({1.f, 2.f, 3.f});
    Groups.Add({2.f, 3.f, 4.f});
    Groups.Add({3.f, 4.f, 5.f});
    const float AvgSim = CalculateAverageSimilarity(Groups);
    UE_LOG(LogTemp, Log, TEXT("Average Similarity: %f"), AvgSim);

    // Demo: Sort Groups by Dissimilarity (using abs(sum(x) - sum(y)))
    TArray<TArray<TArray<float>>> GroupList;
    GroupList.Add( { {1.f, 2.f, 3.f}, {2.f, 3.f, 4.f} } );
    GroupList.Add( { {3.f, 4.f, 5.f}, {4.f, 5.f, 6.f} } );
    const auto Sorted = SortGroupsByDissimilarity(GroupList);
    UE_LOG(LogTemp, Log, TEXT("Groups sorted by dissimilarity! Count: %d"), Sorted.Num());
}

float UInformationProcessorComponent::CalculateRetention(const TArray<float>& Weights, float Alpha, float Time)
{
    if (Weights.Num() == 0)
    {
        return 0.0f;
    }

    const float Decay = FMath::Exp(-Alpha * Time);
    double Sum = 0.0;
    for (const float W : Weights)
    {
        Sum += static_cast<double>(W) * static_cast<double>(Decay);
    }
    return static_cast<float>(Sum / static_cast<double>(Weights.Num()));
}

float UInformationProcessorComponent::CalculateAverageSimilarity(const TArray<TArray<float>>& Vectors)
{
    const int32 N = Vectors.Num();
    if (N < 2)
    {
        return 0.0f;
    }

    int32 PairCount = 0;
    double SimilaritySum = 0.0;

    for (int32 i = 0; i < N; ++i)
    {
        for (int32 j = i + 1; j < N; ++j)
        {
            if (!EnsureSameLength(Vectors[i], Vectors[j]))
            {
                UE_LOG(LogTemp, Warning, TEXT("Vector length mismatch at pairs (%d,%d); skipping."), i, j);
                continue;
            }

            const float Dot = DotProduct(Vectors[i], Vectors[j]);
            const float NormProd = VectorNorm(Vectors[i]) * VectorNorm(Vectors[j]);
            if (NormProd <= KINDA_SMALL_NUMBER)
            {
                UE_LOG(LogTemp, Warning, TEXT("Zero norm encountered at pairs (%d,%d); skipping."), i, j);
                continue;
            }

            SimilaritySum += static_cast<double>(Dot) / static_cast<double>(NormProd);
            ++PairCount;
        }
    }

    if (PairCount == 0)
    {
        return 0.0f;
    }

    return static_cast<float>(SimilaritySum / static_cast<double>(PairCount));
}

TArray<TArray<TArray<float>>> UInformationProcessorComponent::SortGroupsByDissimilarity(const TArray<TArray<TArray<float>>>& Groups)
{
    const int32 G = Groups.Num();
    if (G == 0)
    {
        return {};
    }

    // Precompute dissimilarity sums per group: sum over pairs abs(sum(x) - sum(y))
    TArray<double> DissimSums;
    DissimSums.SetNum(G);

    for (int32 g = 0; g < G; ++g)
    {
        const TArray<TArray<float>>& Group = Groups[g];
        double Accum = 0.0;
        for (int32 i = 0; i < Group.Num(); ++i)
        {
            for (int32 j = i + 1; j < Group.Num(); ++j)
            {
                const double Sx = static_cast<double>(VectorSum(Group[i]));
                const double Sy = static_cast<double>(VectorSum(Group[j]));
                Accum += FMath::Abs(Sx - Sy);
            }
        }
        DissimSums[g] = Accum;
    }

    // Create index array and sort indices by their dissimilarity sums
    TArray<int32> Indices;
    Indices.SetNumUninitialized(G);
    for (int32 i = 0; i < G; ++i) Indices[i] = i;

    Indices.Sort([&](int32 A, int32 B)
    {
        return DissimSums[A] < DissimSums[B];
    });

    // Build sorted output
    TArray<TArray<TArray<float>>> Sorted;
    Sorted.Reserve(G);
    for (int32 idx : Indices)
    {
        Sorted.Add(Groups[idx]);
    }
    return Sorted;
}

// -------------------- Utilities --------------------

bool UInformationProcessorComponent::EnsureSameLength(const TArray<float>& A, const TArray<float>& B)
{
    return A.Num() == B.Num();
}

float UInformationProcessorComponent::DotProduct(const TArray<float>& A, const TArray<float>& B)
{
    check(A.Num() == B.Num());
    double Sum = 0.0;
    for (int32 i = 0; i < A.Num(); ++i)
    {
        Sum += static_cast<double>(A[i]) * static_cast<double>(B[i]);
    }
    return static_cast<float>(Sum);
}

float UInformationProcessorComponent::VectorNorm(const TArray<float>& A)
{
    double SumSq = 0.0;
    for (float V : A)
    {
        SumSq += static_cast<double>(V) * static_cast<double>(V);
    }
    return static_cast<float>(FMath::Sqrt(SumSq));
}

float UInformationProcessorComponent::VectorSum(const TArray<float>& A)
{
    double Sum = 0.0;
    for (float V : A)
    {
        Sum += static_cast<double>(V);
    }
    return static_cast<float>(Sum);
}

