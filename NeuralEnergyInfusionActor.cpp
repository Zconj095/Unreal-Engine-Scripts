#include "NeuralEnergyInfusionActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEnergyInfusionActor::ANeuralEnergyInfusionActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralEnergyInfusionActor::BeginPlay()
{
    Super::BeginPlay();

    float Efficiency = PredictEfficiency(100.f, 200.f);
    UE_LOG(LogTemp, Log, TEXT("Predicted Energy Transfer Efficiency: %.2f"), Efficiency);
}

float ANeuralEnergyInfusionActor::PredictEfficiency(float EnergyOut, float EnergyIn) const
{
    float NormalizedOut = EnergyOut / (EnergyOut + EnergyIn + KINDA_SMALL_NUMBER);
    float NormalizedIn = EnergyIn / (EnergyOut + EnergyIn + KINDA_SMALL_NUMBER);

    float Result = Weights.X * NormalizedOut + Weights.Y * NormalizedIn + Bias;
    return Sigmoid(Result);
}

float ANeuralEnergyInfusionActor::Sigmoid(float Value)
{
    return 1.f / (1.f + FMath::Exp(-Value));
}
