// Unreal conversion of Unity SpatialPartition (octree)

#include "SpatialPartitionActor.h"
#include "DrawDebugHelpers.h"

// Node definition is declared in the public header for codegen completeness

static void BuildSpatialPartitionRecursive(FSpatialNode& Node, int32 Depth, int32 MaxDepth, const TArray<TObjectPtr<AActor>>& Objects)
{
    if (Depth >= MaxDepth)
    {
        return;
    }

    Node.Subdivide();

    for (TUniquePtr<FSpatialNode>& Child : Node.Children)
    {
        if (!Child) continue;
        for (AActor* Obj : Objects)
        {
            if (!IsValid(Obj)) continue;
            const FVector Pos = Obj->GetActorLocation();
            if (Child->ContainsPoint(Pos))
            {
                Child->ContainedObjects.Add(Obj);
            }
        }
        BuildSpatialPartitionRecursive(*Child, Depth + 1, MaxDepth, Objects);
    }
}

static void DrawNodeRecursive(UWorld* World, const FSpatialNode* Node)
{
    if (!World || !Node) return;

    // Draw wire cube equivalent: center + extent
    const FVector Extent(Node->Size * 0.5f);
    DrawDebugBox(World, Node->Center, Extent, FColor::Green, /*bPersistentLines*/ false, /*LifeTime*/ 0.0f, /*DepthPriority*/ 0, /*Thickness*/ 1.0f);

    if (Node->Children.Num() > 0)
    {
        for (const TUniquePtr<FSpatialNode>& Child : Node->Children)
        {
            DrawNodeRecursive(World, Child.Get());
        }
    }
}

ASpatialPartitionActor::ASpatialPartitionActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

ASpatialPartitionActor::~ASpatialPartitionActor() = default;

void ASpatialPartitionActor::BeginPlay()
{
    Super::BeginPlay();

    // Mirror Unity: root centered at origin with InitialSize
    RootNode = MakeUnique<FSpatialNode>(FVector::ZeroVector, InitialSize);
    BuildSpatialPartitionRecursive(*RootNode, /*Depth*/ 0, MaxDepth, ObjectsToPartition);
}

void ASpatialPartitionActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (RootNode)
    {
        DrawNodeRecursive(GetWorld(), RootNode.Get());
    }
}
