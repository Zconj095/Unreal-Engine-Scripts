#include "NeuralEntropicEfficiency3Actor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEntropicEfficiency3Actor::ANeuralEntropicEfficiency3Actor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEntropicEfficiency3Actor::BeginPlay()
{
    Super::BeginPlay();

    EntropicEfficiency = CalculateEntropicEfficiency();
    UE_LOG(LogTemp, Log, TEXT("Initial Entropic Efficiency (ηₛ): %.4f"), EntropicEfficiency);
}

void ANeuralEntropicEfficiency3Actor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    EntropicEfficiency = CalculateEntropicEfficiency();
    UE_LOG(LogTemp, Log, TEXT("Entropic Efficiency (ηₛ): %.4f"), EntropicEfficiency);
}

void ANeuralEntropicEfficiency3Actor::SetTotalBiosyntheticInformationProcessed(float Value)
{
    TotalBiosyntheticInformationProcessed = Value;
}

void ANeuralEntropicEfficiency3Actor::SetNeuralSystemEntropy(float Value)
{
    NeuralSystemEntropy = Value;
}

float ANeuralEntropicEfficiency3Actor::CalculateEntropicEfficiency() const
{
    if (FMath::IsNearlyZero(NeuralSystemEntropy))
    {
        UE_LOG(LogTemp, Warning, TEXT("Entropy (ΔS) cannot be zero."));
        return 0.f;
    }

    return TotalBiosyntheticInformationProcessed / NeuralSystemEntropy;
}
