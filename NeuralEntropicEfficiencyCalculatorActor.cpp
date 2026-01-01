#include "NeuralEntropicEfficiencyCalculatorActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEntropicEfficiencyCalculatorActor::ANeuralEntropicEfficiencyCalculatorActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralEntropicEfficiencyCalculatorActor::BeginPlay()
{
    Super::BeginPlay();

    float Efficiency = CalculateEfficiency();
    if (Efficiency > 0.f)
    {
        UE_LOG(LogTemp, Log, TEXT("Neural Entropic Efficiency (η_s): %.4f"), Efficiency);
    }
}

float ANeuralEntropicEfficiencyCalculatorActor::CalculateEfficiency() const
{
    if (FMath::IsNearlyZero(NeuralEntropy))
    {
        UE_LOG(LogTemp, Error, TEXT("Neural entropy (ΔS) cannot be zero."));
        return 0.f;
    }

    return GeologicalInformation / NeuralEntropy;
}
