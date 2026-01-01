// Unreal conversion of Unity SpatialMemorySystemWithLibraries

#include "SpatialMemorySystemActor.h"
#include "DrawDebugHelpers.h"

// Memory node storing positions of tracked actors
struct FMemoryNode3D
{
    FVector Center = FVector::ZeroVector;
    double Radius = 0.0;

    TMap<TWeakObjectPtr<AActor>, FVector> ObjectMemory;

    FMemoryNode3D() = default;
    FMemoryNode3D(const FVector& InCenter, double InRadius) : Center(InCenter), Radius(InRadius) {}

    void AddOrUpdateObject(AActor* Obj)
    {
        if (!IsValid(Obj)) return;
        ObjectMemory.FindOrAdd(Obj) = Obj->GetActorLocation();
    }

    void RemoveObject(AActor* Obj)
    {
        ObjectMemory.Remove(Obj);
    }

    bool IsWithinBounds(const FVector& Point) const
    {
        return FVector::Dist(Center, Point) <= Radius;
    }

    TArray<AActor*> QueryObjects(const FVector& QueryCenter, double QueryRadius) const
    {
        TArray<AActor*> Result;
        for (const TPair<TWeakObjectPtr<AActor>, FVector>& KVP : ObjectMemory)
        {
            AActor* Actor = KVP.Key.Get();
            if (!IsValid(Actor)) continue;
            const double Dist = FVector::Dist(QueryCenter, KVP.Value);
            if (Dist <= QueryRadius)
            {
                Result.Add(Actor);
            }
        }
        return Result;
    }
};

namespace
{
    TUniquePtr<FMemoryNode3D> GSpatialNode;
}

ASpatialMemorySystemActor::ASpatialMemorySystemActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASpatialMemorySystemActor::BeginPlay()
{
    Super::BeginPlay();

    if (ObjectsToTrack.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("ObjectsToTrack is empty."));
        return;
    }

    GSpatialNode = MakeUnique<FMemoryNode3D>(FVector::ZeroVector, static_cast<double>(MemoryRadius));

    for (AActor* Obj : ObjectsToTrack)
    {
        if (IsValid(Obj))
        {
            GSpatialNode->AddOrUpdateObject(Obj);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("One of the ObjectsToTrack is not valid."));
        }
    }
}

void ASpatialMemorySystemActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!GSpatialNode)
    {
        UE_LOG(LogTemp, Error, TEXT("Spatial memory node is not initialized."));
        return;
    }

    // Update memory for moving objects
    for (AActor* Obj : ObjectsToTrack)
    {
        if (IsValid(Obj))
        {
            GSpatialNode->AddOrUpdateObject(Obj);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("One of the ObjectsToTrack is not valid."));
        }
    }

    // Example query
    const FVector QueryCenter(2.0f, 0.0f, -3.0f);
    const double QueryRange = 7.0;
    const TArray<AActor*> Found = GSpatialNode->QueryObjects(QueryCenter, QueryRange);
    UE_LOG(LogTemp, Log, TEXT("Found %d objects near (%f,%f,%f) within %f units."),
        Found.Num(), QueryCenter.X, QueryCenter.Y, QueryCenter.Z, QueryRange);

    // Draw memory sphere and tracked points
    UWorld* World = GetWorld();
    if (World)
    {
        DrawDebugSphere(World, GSpatialNode->Center, static_cast<float>(GSpatialNode->Radius), 24, FColor::Blue, false, 0.0f, 0, 1.5f);

        for (AActor* Obj : ObjectsToTrack)
        {
            if (!IsValid(Obj)) continue;
            const FVector Pos = Obj->GetActorLocation();
            DrawDebugSphere(World, Pos, 10.0f, 8, FColor::Yellow, false, 0.0f, 0, 1.0f);
        }
    }
}

