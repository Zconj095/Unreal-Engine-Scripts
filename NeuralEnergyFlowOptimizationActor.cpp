#include "NeuralEnergyFlowOptimizationActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEnergyFlowOptimizationActor::ANeuralEnergyFlowOptimizationActor()
{
    PrimaryActorTick.bCanEverTick = false;
    Weights = {0.01f, -0.05f, -0.04f, -0.02f};
}

void ANeuralEnergyFlowOptimizationActor::BeginPlay()
{
    Super::BeginPlay();

    TArray<float> Resistances = {2.f, 3.f, 5.f};
    float TotalEnergy = 100.f;
    float PredictedPower = PredictPower(TotalEnergy, Resistances);
    UE_LOG(LogTemp, Log, TEXT("Predicted Power: %.4f"), PredictedPower);
}

float ANeuralEnergyFlowOptimizationActor::PredictPower(float TotalEnergy, const TArray<float>& Resistances) const
{
    if (Weights.Num() < Resistances.Num() + 1)
    {
        return TotalEnergy * 0.1f;
    }

    float Result = TotalEnergy * Weights[0];
    for (int32 Index = 0; Index < Resistances.Num(); ++Index)
    {
        Result += Resistances[Index] * Weights[Index + 1];
    }

    // simple sigmoid to keep values bounded
    return 1.f / (1.f + FMath::Exp(-Result));
}
