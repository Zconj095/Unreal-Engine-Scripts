#include "NeuralGeosynthesisComplexityActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralGeosynthesisComplexityActor::ANeuralGeosynthesisComplexityActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralGeosynthesisComplexityActor::BeginPlay()
{
    Super::BeginPlay();
    ElapsedTime = 0.f;
    float InitialValue = CalculateComplexity(ElapsedTime);
    UE_LOG(LogTemp, Log, TEXT("Initial Neural Complexity: %.4f"), InitialValue);
}

void ANeuralGeosynthesisComplexityActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    ElapsedTime += DeltaSeconds * TimeStep;
    float Complexity = CalculateComplexity(ElapsedTime);
    UE_LOG(LogTemp, Log, TEXT("Time: %.2f, Neural Complexity: %.4f"), ElapsedTime, Complexity);
}

float ANeuralGeosynthesisComplexityActor::CalculateComplexity(float Time) const
{
    return InitialComplexity * FMath::Exp(GrowthRate * Time);
}
