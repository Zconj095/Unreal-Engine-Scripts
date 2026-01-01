#include "NeuralComplexityCalculatorActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralComplexityCalculatorActor::ANeuralComplexityCalculatorActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralComplexityCalculatorActor::BeginPlay()
{
    Super::BeginPlay();

    float Complexity = CalculateNeuralComplexity(TimeParameter);
    UE_LOG(LogTemp, Log, TEXT("Neural Complexity at time %.2f: %.4f"), TimeParameter, Complexity);
}

float ANeuralComplexityCalculatorActor::CalculateNeuralComplexity(float Time) const
{
    return InitialComplexity * FMath::Exp(GrowthRate * Time);
}
