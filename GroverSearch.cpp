#include "GroverSearch.h"

#include <cmath>

// FQuantumSearchState
FQuantumSearchState::FQuantumSearchState(int32 Size)
{
    const int32 N = FMath::Max(0, Size);
    Amplitudes.SetNumZeroed(N);
    if (N > 0)
    {
        const float Init = 1.0f / FMath::Sqrt(static_cast<float>(N));
        for (int32 i = 0; i < N; ++i)
        {
            Amplitudes[i] = Init;
        }
    }
}

void FQuantumSearchState::ApplyOracle(TFunctionRef<bool(int32)> Predicate)
{
    const int32 N = Amplitudes.Num();
    for (int32 i = 0; i < N; ++i)
    {
        if (Predicate(i))
        {
            Amplitudes[i] = -Amplitudes[i];
        }
    }
}

void FQuantumSearchState::ApplyDiffusion()
{
    const int32 N = Amplitudes.Num();
    if (N <= 0) return;

    double Sum = 0.0;
    for (float A : Amplitudes) Sum += static_cast<double>(A);
    const double Mean = Sum / static_cast<double>(N);

    for (int32 i = 0; i < N; ++i)
    {
        const double Ai = static_cast<double>(Amplitudes[i]);
        const double Reflected = 2.0 * Mean - Ai;
        Amplitudes[i] = static_cast<float>(Reflected);
    }
}

int32 FQuantumSearchState::Measure() const
{
    const int32 N = Amplitudes.Num();
    if (N <= 0) return -1;
    int32 Best = 0;
    double BestProb = static_cast<double>(Amplitudes[0]) * static_cast<double>(Amplitudes[0]);
    for (int32 i = 1; i < N; ++i)
    {
        const double P = static_cast<double>(Amplitudes[i]) * static_cast<double>(Amplitudes[i]);
        if (P > BestProb)
        {
            BestProb = P;
            Best = i;
        }
    }
    return Best;
}

// FVectorDatabaseOracle
FVectorDatabaseOracle::FVectorDatabaseOracle(const TArray<TArray<float>>& InDatabase,
                                             const TArray<float>& InQuery,
                                             float InThreshold)
    : Database(&InDatabase)
    , Query(InQuery)
    , Threshold(InThreshold)
{
}

bool FVectorDatabaseOracle::IsTargetState(int32 Index) const
{
    if (!Database || !Database->IsValidIndex(Index))
    {
        return false;
    }
    const TArray<float>& Vec = (*Database)[Index];
    const float Sim = FEmbeddingSimilarity::ComputeCosineSimilarity(Vec, Query);
    return Sim >= Threshold;
}

// FGroverSearch
int32 FGroverSearch::PerformSearch(const TArray<TArray<float>>& DatabaseVectors,
                                   const TArray<float>& QueryVector,
                                   float SimilarityThreshold,
                                   int32 Iterations)
{
    const int32 Size = DatabaseVectors.Num();
    if (Size <= 0)
    {
        return -1;
    }

    FQuantumSearchState QuantumState(Size);
    FVectorDatabaseOracle Oracle(DatabaseVectors, QueryVector, SimilarityThreshold);

    Iterations = FMath::Max(0, Iterations);
    for (int32 i = 0; i < Iterations; ++i)
    {
        QuantumState.ApplyOracle([&Oracle](int32 StateIndex){ return Oracle.IsTargetState(StateIndex); });
        QuantumState.ApplyDiffusion();
    }

    return QuantumState.Measure();
}

