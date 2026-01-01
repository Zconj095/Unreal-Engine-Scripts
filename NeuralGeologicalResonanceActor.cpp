#include "NeuralGeologicalResonanceActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralGeologicalResonanceActor::ANeuralGeologicalResonanceActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralGeologicalResonanceActor::BeginPlay()
{
    Super::BeginPlay();
    ElapsedTime = 0.f;
    ResonanceFactor = CalculateResonance(ElapsedTime);
    UE_LOG(LogTemp, Log, TEXT("Initial Geological Resonance Factor (Rg): %.4f"), ResonanceFactor);
}

void ANeuralGeologicalResonanceActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ElapsedTime += DeltaSeconds;
    ResonanceFactor = CalculateResonance(ElapsedTime);
    UE_LOG(LogTemp, Log, TEXT("Time: %.2f, Geological Resonance Factor (Rg): %.4f"), ElapsedTime, ResonanceFactor);
}

float ANeuralGeologicalResonanceActor::CalculateResonance(float Time) const
{
    return Amplitude * FMath::Cos((2.f * PI * Time) / Period);
}
