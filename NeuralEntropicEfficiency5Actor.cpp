#include "NeuralEntropicEfficiency5Actor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEntropicEfficiency5Actor::ANeuralEntropicEfficiency5Actor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEntropicEfficiency5Actor::BeginPlay()
{
    Super::BeginPlay();

    EntropicEfficiency = CalculateEntropicEfficiency();
    UE_LOG(LogTemp, Log, TEXT("Initial Entropic Efficiency (ηₙ): %.4f"), EntropicEfficiency);
}

void ANeuralEntropicEfficiency5Actor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    EntropicEfficiency = CalculateEntropicEfficiency();
    UE_LOG(LogTemp, Log, TEXT("Entropic Efficiency (ηₙ): %.4f"), EntropicEfficiency);
}

void ANeuralEntropicEfficiency5Actor::SetTotalInformationProcessed(float Value)
{
    TotalInformationProcessed = Value;
}

void ANeuralEntropicEfficiency5Actor::SetEntropy(float Value)
{
    Entropy = Value;
}

float ANeuralEntropicEfficiency5Actor::CalculateEntropicEfficiency() const
{
    if (FMath::IsNearlyZero(Entropy))
    {
        UE_LOG(LogTemp, Warning, TEXT("Entropy (ΔS) cannot be zero."));
        return 0.f;
    }

    return TotalInformationProcessed / Entropy;
}
