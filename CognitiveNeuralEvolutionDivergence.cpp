#include "CognitiveNeuralEvolutionDivergence.h"

DEFINE_LOG_CATEGORY(LogCognitiveNeuralEvolutionDivergence);

UCognitiveNeuralEvolutionDivergence::UCognitiveNeuralEvolutionDivergence()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCognitiveNeuralEvolutionDivergence::BeginPlay()
{
    Super::BeginPlay();
    RecalculateCNED();
}

void UCognitiveNeuralEvolutionDivergence::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    RecalculateCNED();
    UE_LOG(LogCognitiveNeuralEvolutionDivergence, Verbose, TEXT("CNED: %f"), CnedValue);
}

void UCognitiveNeuralEvolutionDivergence::RecalculateCNED()
{
    if (Omega > KINDA_SMALL_NUMBER)
    {
        CnedValue = DeltaPsi / Omega;
    }
    else
    {
        UE_LOG(LogCognitiveNeuralEvolutionDivergence, Error, TEXT("Omega (Ω) must be greater than zero."));
        CnedValue = 0.f;
    }
}

void UCognitiveNeuralEvolutionDivergence::UpdateDivergence(float Divergence)
{
    DeltaPsi = Divergence;
}

void UCognitiveNeuralEvolutionDivergence::UpdateCognitiveEnergy(float CognitiveEnergy)
{
    if (CognitiveEnergy > KINDA_SMALL_NUMBER)
    {
        Omega = CognitiveEnergy;
    }
    else
    {
        UE_LOG(LogCognitiveNeuralEvolutionDivergence, Error, TEXT("Cognitive energy (Ω) must be greater than zero."));
        Omega = KINDA_SMALL_NUMBER;
    }
}

float UCognitiveNeuralEvolutionDivergence::GetCNED() const
{
    return CnedValue;
}
