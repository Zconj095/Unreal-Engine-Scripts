#include "CosmicLeylineSynchronization.h"

DEFINE_LOG_CATEGORY(LogCosmicLeylineSynchronization);

UCosmicLeylineSynchronization::UCosmicLeylineSynchronization()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCosmicLeylineSynchronization::BeginPlay()
{
    Super::BeginPlay();

    CosmicPower = CalculateCosmicPower();
    NetworkPrediction = PredictCosmicPower();
    TensorSimulation = ComputeTensorLike();

    UE_LOG(LogCosmicLeylineSynchronization, Log, TEXT("Cosmic Power Transferred (P_cosmic): %.6f"), CosmicPower);
    UE_LOG(LogCosmicLeylineSynchronization, Log, TEXT("Perceptron Prediction (P_cosmic): %.6f"), NetworkPrediction);
    UE_LOG(LogCosmicLeylineSynchronization, Log, TEXT("Tensor-style Simulation Result (P_cosmic): %.6f"), TensorSimulation);
}

double UCosmicLeylineSynchronization::CosmicField(double Time) const
{
    return FMath::Sin(Time) + 1.0;
}

double UCosmicLeylineSynchronization::LeylineGradient(double Time) const
{
    return FMath::Cos(Time) + 0.5;
}

double UCosmicLeylineSynchronization::CalculateCosmicPower() const
{
    if (DeltaTime <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCosmicLeylineSynchronization, Warning, TEXT("DeltaTime must be greater than zero."));
        return 0.0;
    }

    const double Start = static_cast<double>(StartTime);
    const double End = static_cast<double>(EndTime);
    if (End <= Start)
    {
        UE_LOG(LogCosmicLeylineSynchronization, Warning, TEXT("EndTime must be greater than StartTime."));
        return 0.0;
    }

    double TotalPower = 0.0;
    for (double Time = Start; Time <= End; Time += DeltaTime)
    {
        const double Phi = CosmicField(Time);
        const double Grad = LeylineGradient(Time);
        TotalPower += Phi * Grad * DeltaTime;
    }
    return TotalPower;
}

double UCosmicLeylineSynchronization::PredictCosmicPower() const
{
    // Simple linear surrogates for the neural network prediction
    const double WeightStart = 0.4;
    const double WeightEnd = 0.6;
    const double Bias = 0.05;
    return StartTime * WeightStart + EndTime * WeightEnd + Bias;
}

double UCosmicLeylineSynchronization::ComputeTensorLike() const
{
    const double Step = FMath::Max(DeltaTime, 0.001f);
    double Accumulated = 0.0;
    for (double Time = StartTime; Time <= EndTime; Time += Step)
    {
        const double Phi = CosmicField(Time);
        const double Grad = LeylineGradient(Time);
        Accumulated += Phi * Grad * Step;
    }
    return Accumulated;
}
