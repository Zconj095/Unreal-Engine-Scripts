#include "CosmicOscillationPotential.h"

DEFINE_LOG_CATEGORY(LogCosmicOscillationPotential);

UCosmicOscillationPotential::UCosmicOscillationPotential()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCosmicOscillationPotential::BeginPlay()
{
    Super::BeginPlay();
    OscillationPotential = InitialFlux;
}

void UCosmicOscillationPotential::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
    OscillationPotential = InitialFlux * FMath::Exp(-DampingCoefficient * CurrentTime) * FMath::Cos(OscillationFrequency * CurrentTime);
    UE_LOG(LogCosmicOscillationPotential, Verbose, TEXT("Cosmic Oscillation Potential (Φ_a): %.6f"), OscillationPotential);
}
