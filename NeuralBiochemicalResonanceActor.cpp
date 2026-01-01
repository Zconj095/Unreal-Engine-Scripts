#include "NeuralBiochemicalResonanceActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralBiochemicalResonanceActor::ANeuralBiochemicalResonanceActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralBiochemicalResonanceActor::BeginPlay()
{
    Super::BeginPlay();

    CurrentTime = 0.f;
    float InitialResonance = CalculateResonance(CurrentTime);
    UE_LOG(LogTemp, Log, TEXT("Initial Neural Biochemical Resonance: %.4f"), InitialResonance);
}

void ANeuralBiochemicalResonanceActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CurrentTime += DeltaSeconds;
    float Resonance = CalculateResonance(CurrentTime);
    UE_LOG(LogTemp, Log, TEXT("Neural Biochemical Resonance (R_b) at time %.2f: %.4f"), CurrentTime, Resonance);
}

float ANeuralBiochemicalResonanceActor::CalculateResonance(float Time) const
{
    return Amplitude * FMath::Cos(2.f * PI * Time / Period);
}
