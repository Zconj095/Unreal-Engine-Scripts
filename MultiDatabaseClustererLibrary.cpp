#include "MultiDatabaseClustererLibrary.h"

#include "InMemoryVectorDatabase.h"
#include "LLMCosineSimilarityVectorDatabase.h"
#include "ClusteringLibrary.h"

void UMultiDatabaseClustererLibrary::ClusterVectors(UMultiDatabaseManager* DbManager, int32 NumClusters, int32 MaxIterations, TArray<FFloatArrayList>& OutClusters)
{
    OutClusters.Reset();
    if (!DbManager || NumClusters <= 0)
    {
        return;
    }

    // Gather vectors from all supported databases
    TArray<FFloatArray> AllVectors;

    const int32 DbCount = DbManager->GetDatabaseCount();
    for (int32 i = 0; i < DbCount; ++i)
    {
        UVectorDatabaseBase* Base = DbManager->GetDatabase(i);
        if (!Base) continue;

        if (UInMemoryVectorDatabase* InMem = Cast<UInMemoryVectorDatabase>(Base))
        {
            TArray<int32> Ids; TArray<FFloatArray> Vecs;
            InMem->GetAllEntries(Ids, Vecs);
            AllVectors.Append(Vecs);
            continue;
        }

        if (ULLMCosineSimilarityVectorDatabase* Vdb = Cast<ULLMCosineSimilarityVectorDatabase>(Base))
        {
            TArray<int32> Ids; TArray<FFloatArray> Vecs;
            Vdb->GetAllVectors(Ids, Vecs);
            AllVectors.Append(Vecs);
            continue;
        }
    }

    if (AllVectors.Num() == 0)
    {
        return;
    }

    // Run KMeans
    UClusteringLibrary::KMeansCluster(AllVectors, NumClusters, MaxIterations, OutClusters);
}

