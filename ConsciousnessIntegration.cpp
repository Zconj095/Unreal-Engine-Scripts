#include "ConsciousnessIntegration.h"

DEFINE_LOG_CATEGORY(LogConsciousnessIntegration);

UConsciousnessIntegration::UConsciousnessIntegration()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UConsciousnessIntegration::BeginPlay()
{
    Super::BeginPlay();

    ConsciousnessIndex = CalculateConsciousnessIndex();
    TensorSimulationResult = ComputeTensorLike(IntegrationTime);

    UE_LOG(LogConsciousnessIntegration, Log, TEXT("Consciousness Complexity Index (C): %.6f"), ConsciousnessIndex);
    UE_LOG(LogConsciousnessIntegration, Log, TEXT("Tensor-like Computation Result (C): %.6f"), TensorSimulationResult);
}

double UConsciousnessIntegration::NeuralActivity(double X) const
{
    return FMath::Sin(X) + 1.0;
}

double UConsciousnessIntegration::MagicalGradient(double X) const
{
    return FMath::Cos(X) + 2.0;
}

double UConsciousnessIntegration::CalculateConsciousnessIndex() const
{
    if (IntegrationTime <= KINDA_SMALL_NUMBER || DX <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogConsciousnessIntegration, Warning, TEXT("IntegrationTime and DX must be positive."));
        return 0.0;
    }

    double Total = 0.0;
    for (double X = 0.0; X <= static_cast<double>(IntegrationTime); X += DX)
    {
        const double Phi = NeuralActivity(X);
        const double Grad = MagicalGradient(X);
        Total += Phi * Grad * DX;
    }

    return Total;
}

double UConsciousnessIntegration::ComputeTensorLike(double UpperBound) const
{
    const double Step = FMath::Max(DX, 0.001f);
    double Sum = 0.0;
    for (double X = 0.0; X <= UpperBound; X += Step)
    {
        const double Phi = NeuralActivity(X);
        const double Grad = MagicalGradient(X);
        Sum += Phi * Grad * Step;
    }
    return Sum;
}
