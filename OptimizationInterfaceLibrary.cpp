#include "OptimizationInterfaceLibrary.h"

#include "HyperstateOptimizer.h"

void UOptimizationInterfaceLibrary::TriggerOptimization(const TArray<FFloatArray>& Origins,
                                                        float LearningRate,
                                                        TArray<float>& OutGlobalFeedback,
                                                        TArray<FFloatArray>& OutUpdatedStates)
{
    OutGlobalFeedback.Reset();
    OutUpdatedStates.Reset();

    if (Origins.Num() == 0)
    {
        return;
    }

    const int32 Dim = Origins[0].Values.Num();
    if (Dim <= 0)
    {
        return;
    }

    // Generate global feedback in [-1,1]
    OutGlobalFeedback.SetNumUninitialized(Dim);
    for (int32 i = 0; i < Dim; ++i)
    {
        OutGlobalFeedback[i] = FMath::FRandRange(-1.f, 1.f);
    }

    // Apply refinement to each origin state
    OutUpdatedStates.Reserve(Origins.Num());
    for (const FFloatArray& State : Origins)
    {
        TArray<float> Updated = FHyperstateOptimizer::RefineState(State.Values, OutGlobalFeedback, LearningRate);
        FFloatArray Wrap; Wrap.Values = MoveTemp(Updated);
        OutUpdatedStates.Add(MoveTemp(Wrap));
    }
}

