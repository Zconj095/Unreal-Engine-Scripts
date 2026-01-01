#include "NeuralEnergyResonanceActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEnergyResonanceActor::ANeuralEnergyResonanceActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEnergyResonanceActor::BeginPlay()
{
    Super::BeginPlay();

    ElapsedTime = 0.0f;
    ResonanceFactor = CalculateResonance(ElapsedTime);
    if (bLogEveryTick)
    {
        UE_LOG(LogTemp, Log, TEXT("Initial Neural Energy Resonance (R_e): %.4f"), ResonanceFactor);
    }
}

void ANeuralEnergyResonanceActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bSimulate)
    {
        return;
    }

    ElapsedTime += DeltaSeconds;
    ResonanceFactor = CalculateResonance(ElapsedTime);

    if (bLogEveryTick)
    {
        UE_LOG(LogTemp, Log, TEXT("Time: %.2f, Neural Energy Resonance (R_e): %.4f"), ElapsedTime, ResonanceFactor);
    }
}

float ANeuralEnergyResonanceActor::CalculateResonance(float Time) const
{
    if (Period <= 0.0f)
    {
        if (!bWarnedAboutPeriod)
        {
            UE_LOG(LogTemp, Warning, TEXT("Period (P) must be greater than zero."));
            bWarnedAboutPeriod = true;
        }
        return 0.0f;
    }

    bWarnedAboutPeriod = false;
    return Amplitude * FMath::Cos((2.f * PI * Time) / Period);
}
