#include "NeuralEntropicEfficiencyActor.h"

ANeuralEntropicEfficiencyActor::ANeuralEntropicEfficiencyActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralEntropicEfficiencyActor::BeginPlay()
{
    Super::BeginPlay();

    float Efficiency = CalculateEntropicEfficiency();
    UE_LOG(LogTemp, Log, TEXT("Neural Entropic Efficiency (η_s): %.4f"), Efficiency);
}

float ANeuralEntropicEfficiencyActor::CalculateEntropicEfficiency() const
{
    if (FMath::IsNearlyZero(Entropy))
    {
        UE_LOG(LogTemp, Warning, TEXT("Entropy (ΔS) is zero or near zero; efficiency cannot be computed."));
        return 0.f;
    }

    return TotalPsychologicalInformation / Entropy;
}
