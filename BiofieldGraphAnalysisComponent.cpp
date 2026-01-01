#include "BiofieldGraphAnalysisComponent.h"

#include "Containers/Queue.h"
#include "Math/UnrealMathUtility.h"

UBiofieldGraphAnalysisComponent::UBiofieldGraphAnalysisComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBiofieldGraphAnalysisComponent::BeginPlay()
{
    Super::BeginPlay();

    BuildGraph(NumVertices);
    const int32 Diameter = CalculateDiameter();
    UE_LOG(LogTemp, Log, TEXT("Graph Diameter: %d"), Diameter);
}

void UBiofieldGraphAnalysisComponent::BuildGraph(int32 InNumVertices)
{
    NumVertices = FMath::Max(0, InNumVertices);
    Graph.Empty();

    // Initialize graph with empty adjacency lists
    for (int32 i = 0; i < NumVertices; ++i)
    {
        Graph.Add(i, FInt32Array());
    }

    // Add undirected edges randomly (probability 0.3)
    for (int32 i = 0; i < NumVertices; ++i)
    {
        for (int32 j = i + 1; j < NumVertices; ++j)
        {
            if (FMath::FRand() < 0.3f)
            {
                Graph[i].Values.Add(j);
                Graph[j].Values.Add(i);
            }
        }
    }
}

int32 UBiofieldGraphAnalysisComponent::CalculateDiameter() const
{
    int32 MaxDiameter = 0;

    for (const TPair<int32, FInt32Array>& Pair : Graph)
    {
        const int32 Source = Pair.Key;
        const TArray<int32> Dist = BreadthFirstSearch(Source);
        int32 CurrentMax = 0;
        for (int32 d : Dist)
        {
            if (d != TNumericLimits<int32>::Max())
            {
                CurrentMax = FMath::Max(CurrentMax, d);
            }
        }
        MaxDiameter = FMath::Max(MaxDiameter, CurrentMax);
    }

    return MaxDiameter;
}

TArray<int32> UBiofieldGraphAnalysisComponent::BreadthFirstSearch(int32 Source) const
{
    TArray<int32> Distances;
    Distances.Init(TNumericLimits<int32>::Max(), NumVertices);

    if (!Graph.Contains(Source))
    {
        return Distances;
    }

    TQueue<int32> Queue;
    Queue.Enqueue(Source);
    Distances[Source] = 0;

    int32 Current;
    while (Queue.Dequeue(Current))
    {
    const FInt32Array* Neighbors = Graph.Find(Current);
    if (!Neighbors) { continue; }

    for (int32 Neighbor : Neighbors->Values)
    {
        if (Neighbor >= 0 && Neighbor < NumVertices && Distances[Neighbor] == TNumericLimits<int32>::Max())
        {
            Distances[Neighbor] = Distances[Current] + 1;
            Queue.Enqueue(Neighbor);
            }
        }
    }

    return Distances;
}
