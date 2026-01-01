#include "NeuralEvolutionaryAdaptabilityActor.h"
#include "Math/UnrealMathUtility.h"

ANeuralEvolutionaryAdaptabilityActor::ANeuralEvolutionaryAdaptabilityActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANeuralEvolutionaryAdaptabilityActor::BeginPlay()
{
    Super::BeginPlay();

    NEA = CalculateNEA();
    UE_LOG(LogTemp, Log, TEXT("Initial NEA: %.4f"), NEA);
}

void ANeuralEvolutionaryAdaptabilityActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    NEA = CalculateNEA();
    UE_LOG(LogTemp, Log, TEXT("NEA: %.4f"), NEA);
}

void ANeuralEvolutionaryAdaptabilityActor::UpdateNeuronGeneration(float NeuronsGenerated)
{
    DeltaNg = NeuronsGenerated;
}

void ANeuralEvolutionaryAdaptabilityActor::UpdateTimeInterval(float TimeInterval)
{
    if (TimeInterval > 0.f)
    {
        DeltaTime = TimeInterval;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Time interval must be greater than zero."));
    }
}

float ANeuralEvolutionaryAdaptabilityActor::GetNEA() const
{
    return NEA;
}

float ANeuralEvolutionaryAdaptabilityActor::CalculateNEA() const
{
    if (DeltaTime <= 0.f)
    {
        UE_LOG(LogTemp, Error, TEXT("Delta Time (Δt) must be greater than zero."));
        return 0.f;
    }

    return Theta * (DeltaNg / DeltaTime);
}
