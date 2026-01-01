#include "NeuralEclipseSynchronizationActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEclipseSynchronizationActor::ANeuralEclipseSynchronizationActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEclipseSynchronizationActor::BeginPlay()
{
    Super::BeginPlay();

    float Synchronization = CalculateSynchronization();
    UE_LOG(LogTemp, Log, TEXT("Initial Neural Synchronization (Se): %.4f"), Synchronization);
}

void ANeuralEclipseSynchronizationActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    float Synchronization = CalculateSynchronization();
    UE_LOG(LogTemp, Log, TEXT("Neural Synchronization (Se): %.4f"), Synchronization);
}

float ANeuralEclipseSynchronizationActor::CalculateSynchronization() const
{
    if (EclipseRange <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Total lunar eclipse range (Rt) must be greater than zero."));
        return 0.f;
    }

    const float AngleFactor = FMath::Cos(PI * EarthShadowRadius / EclipseRange);
    return AngleFactor * EclipseEnergyFlux;
}
