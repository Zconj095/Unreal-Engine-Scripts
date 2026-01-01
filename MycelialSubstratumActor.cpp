#include "MycelialSubstratumActor.h"
#include "DrawDebugHelpers.h"

AMycelialSubstratumActor::AMycelialSubstratumActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMycelialSubstratumActor::BeginPlay()
{
    Super::BeginPlay();

    GenerateMycelialNetwork();

    float MemoryCapacity = CalculateMemoryCapacity(NumberOfConnections, MagicalEfficiency);
    UE_LOG(LogTemp, Log, TEXT("Memory Capacity (M): %.3f"), MemoryCapacity);
}

void AMycelialSubstratumActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bLogMemoryCapacityEveryTick)
    {
        float MemoryCapacity = CalculateMemoryCapacity(NumberOfConnections, MagicalEfficiency);
        UE_LOG(LogTemp, Log, TEXT("Updated Memory Capacity (M): %.3f"), MemoryCapacity);
    }
}

void AMycelialSubstratumActor::GenerateMycelialNetwork()
{
    if (NumberOfConnections <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("NumberOfConnections must be greater than zero."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to generate mycelial network because no world context is available."));
        return;
    }

    NodePositions.Reset();
    NodePositions.Reserve(NumberOfConnections);

    for (int32 Index = 0; Index < NumberOfConnections; ++Index)
    {
        FVector RandomOffset = FMath::VRand() * ConnectionDistance;
        FVector NodeLocation = StartPosition + RandomOffset;
        NodePositions.Add(NodeLocation);

        float ColorLerp = FMath::FRand();
        FLinearColor DebugColor = FLinearColor::LerpUsingHSV(NodeStartColor, NodeEndColor, ColorLerp);
        DrawDebugSphere(World, NodeLocation, NodeSphereRadius, 12, DebugColor.ToFColor(true), true, -1.f, 0, 1.5f);

        if (Index > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, Index - 1);
            ConnectNodes(NodeLocation, NodePositions[RandomIndex]);
        }
    }
}

void AMycelialSubstratumActor::ConnectNodes(const FVector& Start, const FVector& End)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    DrawDebugLine(World, Start, End, FColor::Cyan, true, -1.f, 0, 1.5f);
}

float AMycelialSubstratumActor::CalculateMemoryCapacity(int32 Connections, float Efficiency) const
{
    if (Connections <= 0)
    {
        return 0.f;
    }

    float LogValue = FMath::Loge(static_cast<float>(Connections));
    return Efficiency * LogValue;
}
