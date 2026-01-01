#include "ConsciousnessComplexity.h"

DEFINE_LOG_CATEGORY(LogConsciousnessComplexity);

UConsciousnessComplexity::UConsciousnessComplexity()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UConsciousnessComplexity::BeginPlay()
{
    Super::BeginPlay();

    ConsciousnessIndex = CalculateConsciousnessIndex();
    PerceptronPrediction = PredictWithPerceptron(XStart, XEnd);
    TensorSimulationResult = ComputeTensorLike(XStart, XEnd);

    UE_LOG(LogConsciousnessComplexity, Log, TEXT("Consciousness Index (C): %.6f"), ConsciousnessIndex);
    UE_LOG(LogConsciousnessComplexity, Log, TEXT("Perceptron Prediction (C): %.6f"), PerceptronPrediction);
    UE_LOG(LogConsciousnessComplexity, Log, TEXT("Tensor-like Simulation Result (C): %.6f"), TensorSimulationResult);
}

double UConsciousnessComplexity::GradientAt(double X) const
{
    return FMath::Cos(X) + 1.0;
}

double UConsciousnessComplexity::ComplexityAt(double X) const
{
    return FMath::Sin(X) + 2.0;
}

double UConsciousnessComplexity::CalculateConsciousnessIndex() const
{
    if (DX <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogConsciousnessComplexity, Warning, TEXT("DX must be greater than zero."));
        return 0.0;
    }

    double Total = 0.0;
    for (double X = XStart; X <= XEnd; X += DX)
    {
        const double Grad = GradientAt(X);
        if (FMath::IsNearlyZero(Grad))
        {
            UE_LOG(LogConsciousnessComplexity, Error, TEXT("Energy gradient zero at x=%.3f, skipping interval."), X);
            continue;
        }

        const double Phi = ComplexityAt(X);
        Total += (Phi / Grad) * DX;
    }

    return Total;
}

double UConsciousnessComplexity::PredictWithPerceptron(double StartValue, double EndValue) const
{
    const double WeightX = 0.35;
    const double WeightY = 0.65;
    const double Bias = 0.1;

    return (StartValue * WeightX + EndValue * WeightY + Bias);
}

double UConsciousnessComplexity::ComputeTensorLike(double StartValue, double EndValue) const
{
    const double Step = FMath::Max(DX, 0.001f);
    double Accumulated = 0.0;
    for (double X = StartValue; X <= EndValue; X += Step)
    {
        const double Phi = ComplexityAt(X);
        const double Grad = GradientAt(X);
        if (FMath::IsNearlyZero(Grad))
        {
            continue;
        }
        Accumulated += (Phi / Grad) * Step;
    }

    return Accumulated;
}
