#include "NeuralEnergyComplexityActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEnergyComplexityActor::ANeuralEnergyComplexityActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEnergyComplexityActor::BeginPlay()
{
    Super::BeginPlay();

    TimeElapsed = 0.f;
    Complexity = InitialComplexity;
    UE_LOG(LogTemp, Log, TEXT("Initial Neural Energy Complexity: %.4f"), Complexity);
}

void ANeuralEnergyComplexityActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bSimulate)
    {
        return;
    }

    UpdateComplexity(DeltaSeconds);
    UE_LOG(LogTemp, Log, TEXT("Time: %.2f, Neural Energy Complexity: %.4f"), TimeElapsed, Complexity);
}

void ANeuralEnergyComplexityActor::UpdateComplexity(float DeltaSeconds)
{
    TimeElapsed += DeltaSeconds;
    Complexity = InitialComplexity * FMath::Exp(GrowthRate * TimeElapsed);
}
