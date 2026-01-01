#include "NeuralBiosynthesisComplexityActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralBiosynthesisComplexityActor::ANeuralBiosynthesisComplexityActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralBiosynthesisComplexityActor::BeginPlay()
{
    Super::BeginPlay();

    float Complexity = CalculateBiosynthesisComplexity();
    UE_LOG(LogTemp, Log, TEXT("Neural Biosynthesis Complexity (C_b(t)): %.4f"), Complexity);
}

float ANeuralBiosynthesisComplexityActor::CalculateBiosynthesisComplexity() const
{
    return InitialComplexity * FMath::Exp(GrowthRate * TimeParameter);
}
