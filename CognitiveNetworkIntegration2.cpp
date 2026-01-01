#include "CognitiveNetworkIntegration2.h"

DEFINE_LOG_CATEGORY(LogCognitiveNetworkIntegration2);

UCognitiveNetworkIntegration2::UCognitiveNetworkIntegration2()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveNetworkIntegration2::BeginPlay()
{
    Super::BeginPlay();

    if (Weights.Num() != Inputs.Num())
    {
        UE_LOG(LogCognitiveNetworkIntegration2, Error, TEXT("Weights and inputs arrays must be the same length."));
        CognitiveIntegration = 0.f;
        return;
    }

    CognitiveIntegration = CalculateIntegration();
    UE_LOG(LogCognitiveNetworkIntegration2, Log, TEXT("Cognitive Network Integration (I_a): %f"), CognitiveIntegration);
}

float UCognitiveNetworkIntegration2::CalculateIntegration() const
{
    float Sum = 0.f;
    const int32 Count = FMath::Min(Weights.Num(), Inputs.Num());
    for (int32 Index = 0; Index < Count; ++Index)
    {
        Sum += Weights[Index] * FMath::Loge(1.f + Inputs[Index]);
    }
    return Sum;
}

void UCognitiveNetworkIntegration2::SetWeights(const TArray<float>& NewWeights)
{
    Weights = NewWeights;
    if (Weights.Num() == Inputs.Num())
    {
        CognitiveIntegration = CalculateIntegration();
    }
}

void UCognitiveNetworkIntegration2::SetInputs(const TArray<float>& NewInputs)
{
    Inputs = NewInputs;
    if (Weights.Num() == Inputs.Num())
    {
        CognitiveIntegration = CalculateIntegration();
    }
}
