#include "ContextRetriever.h"

void UContextRetriever::GetRelevantContext(const TArray<float>& InputEmbedding, int32 TopN, TArray<FFloatArray>& OutContextEmbeddings) const
{
    OutContextEmbeddings.Reset();

    if (!MemoryRetrievalSystem || !DbManager || TopN <= 0)
    {
        return;
    }

    TArray<FRetrievalResult> Results;
    MemoryRetrievalSystem->RetrieveRelevantVectors(InputEmbedding, DbManager, TopN, Results);

    for (const FRetrievalResult& R : Results)
    {
        UVectorDatabaseBase* DB = DbManager->GetDatabase(R.DbIndex);
        if (!DB)
        {
            continue;
        }
        TArray<float> Vec;
        DB->GetVector(R.VectorId, Vec);
        if (Vec.Num() > 0)
        {
            FFloatArray Item; Item.Values = MoveTemp(Vec);
            OutContextEmbeddings.Add(MoveTemp(Item));
        }
    }
}

