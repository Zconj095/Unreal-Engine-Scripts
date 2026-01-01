#include "NeuralEfficiencyModelActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEfficiencyModelActor::ANeuralEfficiencyModelActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANeuralEfficiencyModelActor::BeginPlay()
{
    Super::BeginPlay();

    const float Stored = 1e6f;
    const float Input = 2e6f;
    float Efficiency = PredictEfficiency(Stored, Input);
    UE_LOG(LogTemp, Log, TEXT("Predicted Energy Storage Efficiency (Neural): %.2f%%"), Efficiency * 100.f);
}

float ANeuralEfficiencyModelActor::PredictEfficiency(float EnergyStored, float EnergyInput) const
{
    float NormalizedStored = EnergyStored / (EnergyStored + EnergyInput + KINDA_SMALL_NUMBER);
    float NormalizedInput = EnergyInput / (EnergyStored + EnergyInput + KINDA_SMALL_NUMBER);

    float Dot = Weights.X * NormalizedStored + Weights.Y * NormalizedInput + Bias;
    return Sigmoid(Dot);
}

float ANeuralEfficiencyModelActor::Sigmoid(float Value)
{
    return 1.f / (1.f + FMath::Exp(-Value));
}
