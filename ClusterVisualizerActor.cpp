#include "ClusterVisualizerActor.h"

#include "DrawDebugHelpers.h"

AClusterVisualizerActor::AClusterVisualizerActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AClusterVisualizerActor::UpdateClustersFromFloatTriplets(const TArray<FFloatArrayList>& ClusterData)
{
    Clusters.Reset();
    Clusters.Reserve(ClusterData.Num());
    for (const FFloatArrayList& ClusterList : ClusterData)
    {
        FVectorArray OutCluster;
        OutCluster.Points.Reserve(ClusterList.Values.Num());
        for (const FFloatArray& Triplet : ClusterList.Values)
        {
            const TArray<float>& V = Triplet.Values;
            if (V.Num() >= 3)
            {
                OutCluster.Points.Add(FVector(V[0], V[1], V[2]));
            }
        }
        Clusters.Add(MoveTemp(OutCluster));
    }
}

void AClusterVisualizerActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bDrawInGame && GetWorld())
    {
        DrawAll(false, 0.f);
    }
}

void AClusterVisualizerActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    if (bDrawInEditorPersistent && GetWorld())
    {
        FlushPersistentDebugLines(GetWorld());
        DrawAll(true, EditorPersistSeconds);
    }
}

void AClusterVisualizerActor::DrawAll(bool bPersistent, float Duration) const
{
    static const TArray<FColor> Colors = {
        FColor::Red, FColor::Green, FColor::Blue, FColor::Yellow, FColor::Magenta
    };

    const int32 ColorCount = Colors.Num();
    for (int32 i = 0; i < Clusters.Num(); ++i)
    {
        const FColor Color = Colors[i % FMath::Max(1, ColorCount)];
        const FVectorArray& Cluster = Clusters[i];
        for (const FVector& P : Cluster.Points)
        {
            DrawDebugSphere(GetWorld(), P, PointRadius, SphereSegments, Color, bPersistent, Duration, 0, 1.0f);
        }
    }
}
