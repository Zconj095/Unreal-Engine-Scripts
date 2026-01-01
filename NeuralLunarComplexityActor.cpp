#include "NeuralLunarComplexityActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralLunarComplexityActor::ANeuralLunarComplexityActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralLunarComplexityActor::BeginPlay()
{
    Super::BeginPlay();

    ElapsedTime = 0.f;
    Complexity = CalculateLunarComplexity(ElapsedTime);
    UE_LOG(LogTemp, Log, TEXT("Initial Lunar Complexity: %.4f"), Complexity);
}

void ANeuralLunarComplexityActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ElapsedTime += DeltaSeconds;
    Complexity = CalculateLunarComplexity(ElapsedTime);
    UE_LOG(LogTemp, Log, TEXT("Time: %.2f, Lunar Complexity: %.4f"), ElapsedTime, Complexity);
}

float ANeuralLunarComplexityActor::CalculateLunarComplexity(float Time) const
{
    return InitialComplexity * FMath::Exp(GrowthRate * Time);
}
