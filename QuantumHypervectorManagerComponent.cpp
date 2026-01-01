#include "QuantumHypervectorManagerComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/DateTime.h"
#include "Components/SceneComponent.h"

UQuantumHypervectorManagerComponent::UQuantumHypervectorManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    Random.Initialize(FDateTime::Now().GetTicks());
}

void UQuantumHypervectorManagerComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeHypervectors();
    UpdateSceneObjects();
    ClassifyQuadrants();
    DistributeQuantumFeedback();
}

void UQuantumHypervectorManagerComponent::InitializeHypervectors()
{
    Hypervectors.Reset();
    HypervectorActors.Reset();

    if (HypervectorCount <= 0 || HypervectorDimension <= 0)
    {
        return;
    }

    Hypervectors.SetNum(HypervectorCount);

    for (int32 Index = 0; Index < HypervectorCount; ++Index)
    {
        Hypervectors[Index].SetNum(HypervectorDimension);
        for (int32 Dim = 0; Dim < HypervectorDimension; ++Dim)
        {
            Hypervectors[Index][Dim] = FVector(
                Random.FRandRange(-1.0f, 1.0f),
                Random.FRandRange(-1.0f, 1.0f),
                Random.FRandRange(-1.0f, 1.0f)
            );
        }

        if (UWorld* World = GetWorld())
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Name = *FString::Printf(TEXT("Hypervector_%d"), Index);
            AActor* Actor = World->SpawnActor<AActor>(AActor::StaticClass(), GetOwner()->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
            if (Actor)
            {
                Actor->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
                HypervectorActors.Add(Actor);
            }
        }
    }
}

void UQuantumHypervectorManagerComponent::UpdateSceneObjects()
{
    // Placeholder: could update visuals or materials based on scene context.
}

void UQuantumHypervectorManagerComponent::ClassifyQuadrants()
{
    QuadrantClassification.Empty();
    for (int32 Index = 0; Index < 4; ++Index)
    {
        QuadrantClassification.Add(Index, FQuadrantVectorCollection());
    }

    for (const TArray<FVector>& Hypervector : Hypervectors)
    {
        for (const FVector& Vector : Hypervector)
        {
            const int32 Quadrant = GetQuadrant(Vector);
            if (FQuadrantVectorCollection* Collection = QuadrantClassification.Find(Quadrant))
            {
                Collection->Vectors.Add(Vector);
            }
        }
    }
}

int32 UQuantumHypervectorManagerComponent::GetQuadrant(const FVector& Vector) const
{
    if (Vector.X >= 0.0f && Vector.Y >= 0.0f)
    {
        return 0;
    }
    if (Vector.X < 0.0f && Vector.Y >= 0.0f)
    {
        return 1;
    }
    if (Vector.X < 0.0f && Vector.Y < 0.0f)
    {
        return 2;
    }
    return 3;
}

void UQuantumHypervectorManagerComponent::DistributeQuantumFeedback()
{
    for (AActor* Actor : HypervectorActors)
    {
        if (!Actor)
        {
            continue;
        }

        const float Feedback = Random.FRandRange(0.0f, 1.0f);
        const FVector Scale = FVector::OneVector * FMath::Clamp(Feedback, 0.1f, 2.0f);
        Actor->SetActorScale3D(Scale);
    }
}
