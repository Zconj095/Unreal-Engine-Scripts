#include "HyperStateLearner.h"

TSharedPtr<FVectorHyperState> FHyperStateLearner::LearnPattern(const TArray<TSharedPtr<FVectorHyperState>>& Patterns, float LearningRate)
{
    if (Patterns.Num() <= 0 || !Patterns[0].IsValid())
    {
        return MakeShared<FVectorHyperState>();
    }

    // Start with the first pattern
    TSharedPtr<IHyperState> Aggregated = Patterns[0];

    // Superpose remaining patterns
    for (int32 i = 1; i < Patterns.Num(); ++i)
    {
        if (Patterns[i].IsValid())
        {
            Aggregated = Aggregated->Superpose(Patterns[i]);
        }
    }

    // Extract vector and scale by learning rate
    TSharedPtr<FVectorHyperState> AggregatedVec = StaticCastSharedPtr<FVectorHyperState>(Aggregated);
    TArray<float> Adjusted = AggregatedVec.IsValid() ? AggregatedVec->GetVector() : TArray<float>();
    for (float& V : Adjusted)
    {
        V *= LearningRate;
    }

    return MakeShared<FVectorHyperState>(MoveTemp(Adjusted));
}

