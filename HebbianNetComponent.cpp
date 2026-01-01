#include "HebbianNetComponent.h"

UHebbianNetComponent::UHebbianNetComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHebbianNetComponent::Train(const TArray<float>& Inputs)
{
    const int32 N = FMath::Min(Inputs.Num(), Weights.Num());
    for (int32 i = 0; i < N; ++i)
    {
        Weights[i] += Inputs[i]; // Simple Hebbian update
    }
}

