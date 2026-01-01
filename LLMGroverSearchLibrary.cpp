#include "LLMGroverSearchLibrary.h"

#include "GroverSearch.h"

int32 ULLMGroverSearchLibrary::PerformSearchCosineThreshold(const TArray<FFloatArray>& Dataset,
                                                            const TArray<float>& Query,
                                                            float SimilarityThreshold)
{
    // Convert Dataset to array of raw float arrays expected by FGroverSearch
    TArray<TArray<float>> Database;
    Database.Reserve(Dataset.Num());
    for (const FFloatArray& Row : Dataset)
    {
        Database.Add(Row.Values);
    }

    const int32 Dimensions = Query.Num();
    const int32 Iterations = FMath::Max(0, static_cast<int32>(FMath::FloorToFloat(FMath::Sqrt(static_cast<float>(Dimensions)))));
    return FGroverSearch::PerformSearch(Database, Query, SimilarityThreshold, Iterations);
}

