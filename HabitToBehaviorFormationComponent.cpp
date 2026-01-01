#include "HabitToBehaviorFormationComponent.h"

UHabitToBehaviorFormationComponent::UHabitToBehaviorFormationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UHabitToBehaviorFormationComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initial computation (mirrors Unity Start)
    ComputeBehaviorStrength();
}

void UHabitToBehaviorFormationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Dynamic update (mirrors Unity Update)
    ComputeBehaviorStrength();
}

void UHabitToBehaviorFormationComponent::ComputeBehaviorStrength()
{
    const int32 Num = HabitStrengths.Num();
    if (Num == 0 || HabitWeights.Num() != Num)
    {
        UE_LOG(LogTemp, Error, TEXT("[HabitToBehaviorFormation] Habit strengths and weights must be non-empty and of the same length!"));
        return;
    }

    double Sum = 0.0;
    for (int32 i = 0; i < Num; ++i)
    {
        Sum += HabitStrengths[i] * HabitWeights[i];
    }
    BehaviorStrength = Sum;

    UE_LOG(LogTemp, Log, TEXT("[HabitToBehaviorFormation] Behavior Strength (B): %f"), (float)BehaviorStrength);
}

double UHabitToBehaviorFormationComponent::PredictBehaviorStrength(const TArray<double>& Inputs) const
{
    // Placeholder: Replace with ML integration as needed.
    // For now, return the currently computed behavior strength.
    return BehaviorStrength;
}

void UHabitToBehaviorFormationComponent::UpdateHabitsAndWeights(const TArray<double>& NewHabitStrengths, const TArray<double>& NewHabitWeights)
{
    if (NewHabitStrengths.Num() != NewHabitWeights.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("[HabitToBehaviorFormation] New habit strengths and weights must have the same length!"));
        return;
    }

    HabitStrengths = NewHabitStrengths;
    HabitWeights = NewHabitWeights;
}

