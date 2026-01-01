#include "HabitFormationComponent.h"
#include "Engine/Engine.h"

UHabitFormationComponent::UHabitFormationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UHabitFormationComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initial computation of habit strength (mirrors Unity Start)
    ComputeAndLogHabitStrength();
}

void UHabitFormationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Continuously update the habit strength dynamically (mirrors Unity Update)
    ComputeAndLogHabitStrength();
}

void UHabitFormationComponent::ComputeAndLogHabitStrength()
{
    const int32 Num = Rewards.Num();
    if (Num == 0 || ActionConsistencies.Num() != Num || Times.Num() != Num)
    {
        UE_LOG(LogTemp, Error, TEXT("[HabitFormation] Input arrays must be non-empty and of the same length!"));
        return;
    }

    HabitStrength = InitialHabitStrength;

    for (int32 i = 0; i < Num; ++i)
    {
        const double TimeDifference = CurrentTime - Times[i];
        const double ExponentialDecay = FMath::Exp(-DecayRate * TimeDifference);
        HabitStrength += LearningRate * Rewards[i] * ActionConsistencies[i] * ExponentialDecay;
    }

    UE_LOG(LogTemp, Log, TEXT("[HabitFormation] Habit Strength at time %f: %f"), (float)CurrentTime, (float)HabitStrength);
}

double UHabitFormationComponent::PredictHabitStrength(const TArray<double>& Inputs) const
{
    // Placeholder: In Unity, this used Accord.Neuro's ActivationNetwork.
    // In Unreal, integrate your ML framework of choice here.
    // For now, return current HabitStrength as a stub.
    return HabitStrength;
}

