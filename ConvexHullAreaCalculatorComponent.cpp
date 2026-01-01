#include "ConvexHullAreaCalculatorComponent.h"

#include "DrawDebugHelpers.h"

UConvexHullAreaCalculatorComponent::UConvexHullAreaCalculatorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Default points similar to Unity example
    Points = {
        FVector2D(1.f, 3.f), FVector2D(3.f, 4.f), FVector2D(5.f, 7.f),
        FVector2D(3.f, 1.f), FVector2D(6.f, 7.f), FVector2D(5.f, 2.f)
    };
}

float UConvexHullAreaCalculatorComponent::CalculateArea()
{
    const TArray<FVector2D> Hull = FindConvexHull(Points);
    return CalculatePolygonArea(Hull);
}

TArray<FVector2D> UConvexHullAreaCalculatorComponent::FindConvexHull(const TArray<FVector2D>& InPoints) const
{
    TArray<FVector2D> Hull;
    if (InPoints.Num() == 0)
    {
        return Hull;
    }

    // Find the leftmost point
    FVector2D StartPoint = InPoints[0];
    for (const FVector2D& Pt : InPoints)
    {
        if (Pt.X < StartPoint.X)
        {
            StartPoint = Pt;
        }
    }

    FVector2D CurrentPoint = StartPoint;
    FVector2D EndPoint;
    do
    {
        Hull.Add(CurrentPoint);
        EndPoint = InPoints[0];

        for (int32 i = 1; i < InPoints.Num(); ++i)
        {
            const FVector2D& Candidate = InPoints[i];
            if (EndPoint == CurrentPoint || IsCounterClockwise(CurrentPoint, EndPoint, Candidate))
            {
                EndPoint = Candidate;
            }
        }
        CurrentPoint = EndPoint;
    }
    while (!(EndPoint == StartPoint));

    return Hull;
}

bool UConvexHullAreaCalculatorComponent::IsCounterClockwise(const FVector2D& A, const FVector2D& B, const FVector2D& C) const
{
    return (B.X - A.X) * (C.Y - A.Y) - (B.Y - A.Y) * (C.X - A.X) > 0.f;
}

float UConvexHullAreaCalculatorComponent::CalculatePolygonArea(const TArray<FVector2D>& Polygon) const
{
    if (Polygon.Num() < 3)
    {
        return 0.f;
    }

    float Area = 0.f;
    int32 J = Polygon.Num() - 1;
    for (int32 I = 0; I < Polygon.Num(); ++I)
    {
        // Mirror Unity's formula: (x_j + x_i) * (y_j - y_i)
        Area += (Polygon[J].X + Polygon[I].X) * (Polygon[J].Y - Polygon[I].Y);
        J = I;
    }

    return FMath::Abs(Area / 2.0f);
}

void UConvexHullAreaCalculatorComponent::DrawHullDebug(float ZHeight, float PointRadius, float Duration) const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    const TArray<FVector2D> Hull = FindConvexHull(Points);
    const int32 N = Hull.Num();
    if (N == 0)
    {
        return;
    }

    // Draw points and lines on XY plane at ZHeight
    for (int32 i = 0; i < N; ++i)
    {
        const FVector A(Hull[i].X, Hull[i].Y, ZHeight);
        const FVector B(Hull[(i + 1) % N].X, Hull[(i + 1) % N].Y, ZHeight);

        DrawDebugSphere(World, A, PointRadius, 8, FColor::Red, Duration > 0.f, Duration);
        DrawDebugLine(World, A, B, FColor::Red, Duration > 0.f, Duration, 0, 1.0f);
    }
}

