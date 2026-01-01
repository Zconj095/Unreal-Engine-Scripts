#include "PatternPreallocationComponent.h"
#include "PreallocatedComponent.h"
#include "Kismet/GameplayStatics.h"

UPatternPreallocationComponent::UPatternPreallocationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPatternPreallocationComponent::BeginPlay()
{
    Super::BeginPlay();
    ExecuteAllocations();
}

void UPatternPreallocationComponent::ExecuteAllocations()
{
    Allocations.Reset();

    TArray<AActor*> Entities;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Entities);

    int32 Total = Entities.Num();
    for (int32 Index = 0; Index < Total; ++Index)
    {
        if (AActor* Actor = Entities[Index])
        {
            if (Actor->FindComponentByClass<UPreallocatedComponent>())
            {
                continue;
            }

            FPatternAllocationData Data;
            Data.Entity = Actor;
            Data.AllocatedPosition = CalculatePosition(Total, Allocations.Num());
            Allocations.Add(Data);
        }
    }

    ApplyAllocations();
}

FVector UPatternPreallocationComponent::CalculatePosition(int32 TotalEntities, int32 Index) const
{
    const float AngleStep = 2.f * PI;
    switch (AllocationPattern)
    {
    case EAllocationPattern::Grid:
    {
        const int32 GridSize = FMath::Max(1, FMath::CeilToInt(FMath::Sqrt(static_cast<float>(TotalEntities))));
        const int32 Row = Index / GridSize;
        const int32 Column = Index % GridSize;
        return CenterPoint + FVector(Column * GridSpacing.X, Row * GridSpacing.Y, 0.f);
    }
    case EAllocationPattern::Circular:
    {
        const float Angle = AngleStep * Index / TotalEntities;
        return CenterPoint + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * CircleRadius;
    }
    case EAllocationPattern::Radial:
    {
        const int32 Segment = Index % RadialSegments;
        const float RadiusStep = CircleRadius * (Index / (float)RadialSegments);
        const float Angle = AngleStep * Segment / RadialSegments;
        return CenterPoint + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * RadiusStep;
    }
    case EAllocationPattern::Random:
    {
        return CenterPoint + FVector(
            FMath::FRandRange(-RandomAreaSize, RandomAreaSize),
            FMath::FRandRange(-RandomAreaSize, RandomAreaSize),
            0.f
        );
    }
    default:
        return CenterPoint;
    }
}

void UPatternPreallocationComponent::ApplyAllocations()
{
    for (const FPatternAllocationData& Allocation : Allocations)
    {
        if (Allocation.Entity)
        {
            Allocation.Entity->SetActorLocation(Allocation.AllocatedPosition);
        }
    }
}
