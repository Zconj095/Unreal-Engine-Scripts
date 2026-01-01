#include "MysticalLunarSynchronizationActor.h"
#include "Math/UnrealMathUtility.h"

AMysticalLunarSynchronizationActor::AMysticalLunarSynchronizationActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMysticalLunarSynchronizationActor::BeginPlay()
{
    Super::BeginPlay();

    if (SampleCount < 2)
    {
        SampleCount = 2;
        UE_LOG(LogTemp, Warning, TEXT("SampleCount was too low; raising to 2 for integration."));
    }

    if (TimeEnd <= TimeStart)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeEnd must be greater than TimeStart; skipping integration."));
        return;
    }

    double LunarEnergyFlow = IntegrateLunarEnergy(TimeStart, TimeEnd, SampleCount);
    UE_LOG(LogTemp, Log, TEXT("Mystical lunar energy flow (P_lunar): %.4f"), LunarEnergyFlow);
}

double AMysticalLunarSynchronizationActor::IntegrateLunarEnergy(double InTimeStart, double InTimeEnd, int32 InSampleCount) const
{
    const double IntervalLength = InTimeEnd - InTimeStart;
    const double StepSize = IntervalLength / InSampleCount;

    auto Integrand = [this](double Time) { return Phi(Time) * GradientPsi(Time); };

    double Accumulated = 0.0;
    for (int32 Index = 1; Index < InSampleCount; ++Index)
    {
        double SampleTime = InTimeStart + Index * StepSize;
        Accumulated += Integrand(SampleTime);
    }

    double StartValue = Integrand(InTimeStart);
    double EndValue = Integrand(InTimeEnd);

    return (StepSize * (StartValue + EndValue + 2.0 * Accumulated)) * 0.5;
}

double AMysticalLunarSynchronizationActor::Phi(double Time) const
{
    return FMath::Sin(Time);
}

double AMysticalLunarSynchronizationActor::GradientPsi(double Time) const
{
    return FMath::Cos(Time);
}
