#include "NeuralEntropicEfficiency7Actor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEntropicEfficiency7Actor::ANeuralEntropicEfficiency7Actor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEntropicEfficiency7Actor::BeginPlay()
{
    Super::BeginPlay();

    EtaS = CalculateNEEB();
    UE_LOG(LogTemp, Log, TEXT("Initial ηs: %.2f"), EtaS);
}

void ANeuralEntropicEfficiency7Actor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    EtaS = CalculateNEEB();
    UE_LOG(LogTemp, Log, TEXT("Entropic Efficiency (ηs): %.2f"), EtaS);
}

float ANeuralEntropicEfficiency7Actor::CalculateNEEB() const
{
    if (FMath::IsNearlyZero(Entropy))
    {
        UE_LOG(LogTemp, Warning, TEXT("Entropy (ΔS) cannot be zero. Returning efficiency as 0."));
        return 0.f;
    }

    return TotalInformation / Entropy;
}
