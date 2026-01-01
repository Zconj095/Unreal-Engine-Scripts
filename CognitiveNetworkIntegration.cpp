#include "CognitiveNetworkIntegration.h"

DEFINE_LOG_CATEGORY(LogCognitiveNetworkIntegration);

UCognitiveNetworkIntegration::UCognitiveNetworkIntegration()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveNetworkIntegration::BeginPlay()
{
    Super::BeginPlay();

    if (Weights.Num() != Inputs.Num())
    {
        UE_LOG(LogCognitiveNetworkIntegration, Error, TEXT("Weights and inputs arrays must have the same length."));
        CognitiveIntegration = 0.f;
        return;
    }

    CognitiveIntegration = CalculateIntegration();
    UE_LOG(LogCognitiveNetworkIntegration, Log, TEXT("Cognitive Network Integration (I_a): %f"), CognitiveIntegration);
}

float UCognitiveNetworkIntegration::CalculateIntegration() const
{
    float Sum = 0.f;
    const int32 NumElements = FMath::Min(Weights.Num(), Inputs.Num());
    for (int32 Index = 0; Index < NumElements; ++Index)
    {
        Sum += Weights[Index] * FMath::Loge(1.f + Inputs[Index]);
    }

    return Sum;
}

void UCognitiveNetworkIntegration::SetWeights(const TArray<float>& NewWeights)
{
    Weights = NewWeights;
    if (Weights.Num() == Inputs.Num())
    {
        CognitiveIntegration = CalculateIntegration();
    }
}

void UCognitiveNetworkIntegration::SetInputs(const TArray<float>& NewInputs)
{
    Inputs = NewInputs;
    if (Weights.Num() == Inputs.Num())
    {
        CognitiveIntegration = CalculateIntegration();
    }
}
