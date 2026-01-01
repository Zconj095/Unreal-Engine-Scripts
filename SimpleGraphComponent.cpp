#include "SimpleGraphComponent.h"

USimpleGraphComponent::USimpleGraphComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USimpleGraphComponent::BeginPlay()
{
    Super::BeginPlay();

    // Example usage mirroring Unity Start()
    for (int32 i = 0; i < 10; ++i)
    {
        AddVertex(i);
    }

    AddEdge(0, 1);
    AddEdge(1, 2);
    AddEdge(2, 3);
    AddEdge(4, 5);
    AddEdge(5, 6);
    AddEdge(7, 8);
    AddEdge(8, 9);

    const int32 NumCommunities = DetectCommunities();
    UE_LOG(LogTemp, Log, TEXT("Number of communities: %d"), NumCommunities);
}

void USimpleGraphComponent::AddVertex(int32 Vertex)
{
    Adjacency.FindOrAdd(Vertex); // ensures key exists with empty array
}

void USimpleGraphComponent::AddEdge(int32 Vertex1, int32 Vertex2)
{
    if (Adjacency.Contains(Vertex1) && Adjacency.Contains(Vertex2))
    {
        Adjacency.FindOrAdd(Vertex1).Values.Add(Vertex2);
        Adjacency.FindOrAdd(Vertex2).Values.Add(Vertex1);
    }
}

int32 USimpleGraphComponent::DetectCommunities() const
{
    TSet<int32> Visited;
    int32 NumCommunities = 0;

    for (const TPair<int32, FInt32Array>& Pair : Adjacency)
    {
        const int32 Vertex = Pair.Key;
        if (!Visited.Contains(Vertex))
        {
            // Start DFS from this vertex marking new community
            // Cast away const for DFS helper that doesn't modify Adjacency
            TSet<int32> LocalVisited = Visited;
            const_cast<USimpleGraphComponent*>(this)->DFS(Vertex, LocalVisited);
            Visited = MoveTemp(LocalVisited);
            ++NumCommunities;
        }
    }

    return NumCommunities;
}

void USimpleGraphComponent::DFS(int32 StartVertex, TSet<int32>& Visited) const
{
    TArray<int32> Stack;
    Stack.Push(StartVertex);
    Visited.Add(StartVertex);

    while (Stack.Num() > 0)
    {
        const int32 Current = Stack.Pop(EAllowShrinking::No);
        if (const FInt32Array* Neighbors = Adjacency.Find(Current))
        {
            for (int32 Neighbor : Neighbors->Values)
            {
                if (!Visited.Contains(Neighbor))
                {
                    Visited.Add(Neighbor);
                    Stack.Push(Neighbor);
                }
            }
        }
    }
}
