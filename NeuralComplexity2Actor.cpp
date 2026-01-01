#include "NeuralComplexity2Actor.h"
#include "Math/UnrealMathUtility.h"

ANeuralComplexity2Actor::ANeuralComplexity2Actor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralComplexity2Actor::BeginPlay()
{
    Super::BeginPlay();

    TimeElapsed = 0.f;
    float InitialValue = CalculateNeuralComplexity(TimeElapsed);
    UE_LOG(LogTemp, Log, TEXT("Initial Neural Complexity: %.4f"), InitialValue);
}

void ANeuralComplexity2Actor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    TimeElapsed += DeltaSeconds;
    float Complexity = CalculateNeuralComplexity(TimeElapsed);
    UE_LOG(LogTemp, Log, TEXT("Time: %.2f, Neural Complexity: %.4f"), TimeElapsed, Complexity);
}

float ANeuralComplexity2Actor::CalculateNeuralComplexity(float Time) const
{
    return InitialNeuralComplexity * FMath::Exp(GrowthRate * Time);
}
