#include "CosmicNeuralPlasticity.h"

DEFINE_LOG_CATEGORY(LogCosmicNeuralPlasticity);

UCosmicNeuralPlasticity::UCosmicNeuralPlasticity()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCosmicNeuralPlasticity::BeginPlay()
{
    Super::BeginPlay();

    if (EnergyAmplitudes.Num() != CelestialSourcesCount ||
        Frequencies.Num() != CelestialSourcesCount ||
        PhaseShifts.Num() != CelestialSourcesCount)
    {
        UE_LOG(LogCosmicNeuralPlasticity, Error, TEXT("Array lengths must match CelestialSourcesCount."));
        return;
    }

    DeltaNeuralPlasticity = CalculateCosmicNeuralPlasticity();
    UE_LOG(LogCosmicNeuralPlasticity, Log, TEXT("?N_cosmic after ?=%.1fs: %f"), CosmicDuration, DeltaNeuralPlasticity);
}

float UCosmicNeuralPlasticity::CalculateCosmicNeuralPlasticity() const
{
    if (CelestialSourcesCount <= 0 || CosmicDuration <= KINDA_SMALL_NUMBER)
    {
        return 0.f;
    }

    const int32 Steps = 1000;
    const double Dt = static_cast<double>(CosmicDuration) / Steps;
    double Integral = 0.0;

    for (int32 Index = 0; Index < Steps; ++Index)
    {
        const double Time = Index * Dt;
        const double Resonance = CalculateCelestialResonance(Time);
        Integral += PlasticityCoefficient * Resonance * Dt;
    }

    return static_cast<float>(Integral);
}

float UCosmicNeuralPlasticity::CalculateCelestialResonance(double TimeSeconds) const
{
    double Sum = 0.0;
    for (int32 Index = 0; Index < CelestialSourcesCount; ++Index)
    {
        const double Amplitude = EnergyAmplitudes.IsValidIndex(Index) ? EnergyAmplitudes[Index] : 0.0;
        const double Frequency = Frequencies.IsValidIndex(Index) ? Frequencies[Index] : 0.0;
        const double Phase = PhaseShifts.IsValidIndex(Index) ? PhaseShifts[Index] : 0.0;
        Sum += Amplitude * FMath::Cos(2.0 * PI * Frequency * TimeSeconds + Phase);
    }

    return static_cast<float>(Sum);
}
