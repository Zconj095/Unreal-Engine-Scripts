#include "NeuralAstronomyComplexityActor.h"

ANeuralAstronomyComplexityActor::ANeuralAstronomyComplexityActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralAstronomyComplexityActor::BeginPlay()
{
    Super::BeginPlay();

    CurrentTime = 0.f;
    float InitialValue = CalculateNeuralComplexity(CurrentTime);
    UE_LOG(LogTemp, Log, TEXT("Initial Neural Astronomy Complexity: %.4f"), InitialValue);
}

void ANeuralAstronomyComplexityActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CurrentTime += DeltaSeconds;
    float Complexity = CalculateNeuralComplexity(CurrentTime);
    UE_LOG(LogTemp, Log, TEXT("Neural Complexity at time %.2f: %.4f"), CurrentTime, Complexity);
}

float ANeuralAstronomyComplexityActor::CalculateNeuralComplexity(float Time) const
{
    return InitialComplexity * FMath::Exp(GrowthRate * Time);
}
