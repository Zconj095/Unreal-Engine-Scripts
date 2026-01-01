#include "HyperstateOptimizer.h"

TArray<float> FHyperstateOptimizer::RefineState(const TArray<float>& State, const TArray<float>& Feedback, float LearningRate)
{
    const int32 N = State.Num();
    TArray<float> Optimized;
    Optimized.SetNumUninitialized(N);

    for (int32 i = 0; i < N; ++i)
    {
        const float Fb = (i < Feedback.Num()) ? Feedback[i] : 0.0f;
        Optimized[i] = State[i] + LearningRate * Fb;
    }

    return Optimized;
}

TArray<float> FHyperstateOptimizer::NormalizeState(const TArray<float>& State)
{
    double SumSq = 0.0;
    for (float V : State)
    {
        SumSq += static_cast<double>(V) * static_cast<double>(V);
    }

    const float Magnitude = FMath::Sqrt(static_cast<float>(SumSq));
    if (Magnitude <= KINDA_SMALL_NUMBER)
    {
        return State; // avoid division by zero; return original
    }

    TArray<float> Normalized;
    Normalized.SetNumUninitialized(State.Num());
    for (int32 i = 0; i < State.Num(); ++i)
    {
        Normalized[i] = State[i] / Magnitude;
    }
    return Normalized;
}

