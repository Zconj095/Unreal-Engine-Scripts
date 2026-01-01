#include "NeuralFieldDynamicsComponent.h"

#include "Math/UnrealMathUtility.h"

UNeuralFieldDynamicsComponent::UNeuralFieldDynamicsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

TArray<float> UNeuralFieldDynamicsComponent::Spread(float V, float W, float t) const
{
    const float dVdt = (V - FMath::Pow(V, 3.0f) / 3.0f - W + Iext) / Tau;
    const float dWdt = Gamma * (V - Beta * W) / Tau;

    TArray<float> Out; Out.SetNum(2);
    Out[0] = dVdt;
    Out[1] = dWdt;
    return Out;
}

