#include "ComplexityVisualizer.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogComplexityVisualizer);

UComplexityVisualizer::UComplexityVisualizer()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UComplexityVisualizer::BeginPlay()
{
    Super::BeginPlay();

    VisualizeComplexity();
}

void UComplexityVisualizer::VisualizeComplexity() const
{
    if (!PointPrefab)
    {
        UE_LOG(LogComplexityVisualizer, Warning, TEXT("PointPrefab is not assigned; visualization aborted."));
        return;
    }

    if (!GetWorld())
    {
        UE_LOG(LogComplexityVisualizer, Warning, TEXT("World context missing; cannot spawn visualization points."));
        return;
    }

    const FVector Origin = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
    for (int32 InputSize = 1; InputSize <= MaxInputSize; ++InputSize)
    {
        const double Complexity = OptimizationParameters.ComputeTimeComplexity(InputSize);
        const FVector SpawnLocation = Origin + FVector(InputSize * HorizontalSpacing, static_cast<float>(Complexity) * ComplexityScale, 0.f);

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        if (AActor* SpawnedPoint = GetWorld()->SpawnActor<AActor>(PointPrefab, SpawnLocation, FRotator::ZeroRotator, SpawnParams))
        {
            const float ScaleValue = FMath::Max(0.1f, static_cast<float>(Complexity) * 0.5f);
            SpawnedPoint->SetActorScale3D(FVector(0.5f, ScaleValue, 0.5f));
        }
    }

    UE_LOG(LogComplexityVisualizer, Log, TEXT("Spawned %d complexity points with horizontal spacing %.1f."), MaxInputSize, HorizontalSpacing);
}
