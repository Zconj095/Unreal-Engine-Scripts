#include "HyperthreadedOptimizer.h"
#include "Async/ParallelFor.h"

void FHyperthreadedOptimizer::OptimizeParallel(const TArray<TSharedPtr<IMultiStateVector>>& Vectors,
                                               const TArray<float>& Feedback,
                                               float LearningRate)
{
    if (Vectors.Num() <= 0)
    {
        return;
    }

    ParallelFor(Vectors.Num(), [&](int32 Index)
    {
        const TSharedPtr<IMultiStateVector>& Item = Vectors[Index];
        if (Item.IsValid())
        {
            Item->Optimize(Feedback, LearningRate);
        }
    });
}

