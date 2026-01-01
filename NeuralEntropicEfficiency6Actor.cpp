#include "NeuralEntropicEfficiency6Actor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEntropicEfficiency6Actor::ANeuralEntropicEfficiency6Actor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEntropicEfficiency6Actor::BeginPlay()
{
    Super::BeginPlay();

    EntropicEfficiency = CalculateEfficiency();
    UE_LOG(LogTemp, Log, TEXT("Initial Entropic Efficiency (η_s): %.2f"), EntropicEfficiency);
}

void ANeuralEntropicEfficiency6Actor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    EntropicEfficiency = CalculateEfficiency();
    UE_LOG(LogTemp, Log, TEXT("Entropic Efficiency (η_s): %.2f"), EntropicEfficiency);
}

float ANeuralEntropicEfficiency6Actor::CalculateEfficiency() const
{
    if (FMath::IsNearlyZero(Entropy))
    {
        UE_LOG(LogTemp, Warning, TEXT("Entropy (ΔS) is zero, returning infinite efficiency."));
        return TNumericLimits<float>::Max();
    }

    return TotalInformation / Entropy;
}
