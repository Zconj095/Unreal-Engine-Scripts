#include "SimpleSynergyRetrievalSystem.h"

#include "EmbeddingSimilarity.h"
#include "MultiDatabaseManager.h"
#include "InMemoryVectorDatabase.h"

void USimpleSynergyRetrievalSystem::RetrieveRelevantVectors_Implementation(const TArray<float>& InputEmbedding, UMultiDatabaseManager* DbManager, int32 TopN, TArray<FRetrievalResult>& OutResults) const
{
    OutResults.Reset();
    if (!DbManager || InputEmbedding.Num() == 0 || TopN <= 0)
    {
        return;
    }

    struct FScored { int32 DbIndex; int32 VectorId; float Score; };
    TArray<FScored> Scored;

    const int32 DbCount = DbManager->GetDatabaseCount();
    for (int32 dbi = 0; dbi < DbCount; ++dbi)
    {
        UVectorDatabaseBase* Base = DbManager->GetDatabase(dbi);
        UInMemoryVectorDatabase* InMem = Cast<UInMemoryVectorDatabase>(Base);
        if (!InMem)
        {
            continue; // unsupported DB type
        }

        TArray<int32> Ids; TArray<FFloatArray> Vecs;
        InMem->GetAllEntries(Ids, Vecs);
        const int32 N = FMath::Min(Ids.Num(), Vecs.Num());
        for (int32 i = 0; i < N; ++i)
        {
            const float S = FEmbeddingSimilarity::ComputeCosineSimilarity(InputEmbedding, Vecs[i].Values);
            Scored.Add({ dbi, Ids[i], S });
        }
    }

    if (Scored.Num() == 0)
    {
        return;
    }

    Scored.Sort([](const FScored& A, const FScored& B){ return A.Score > B.Score; });

    const int32 Count = FMath::Min(TopN, Scored.Num());
    OutResults.Reserve(Count);
    for (int32 i = 0; i < Count; ++i)
    {
        FRetrievalResult R; R.DbIndex = Scored[i].DbIndex; R.VectorId = Scored[i].VectorId; R.Score = Scored[i].Score;
        OutResults.Add(R);
    }
}

