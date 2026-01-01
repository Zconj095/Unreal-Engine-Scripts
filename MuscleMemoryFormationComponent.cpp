#include "MuscleMemoryFormationComponent.h"

UMuscleMemoryFormationComponent::UMuscleMemoryFormationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMuscleMemoryFormationComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initial computation (mirrors Unity Start)
    ComputeMuscleMemoryStrength();
}

void UMuscleMemoryFormationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Dynamic update (mirrors Unity Update)
    ComputeMuscleMemoryStrength();
}

void UMuscleMemoryFormationComponent::ComputeMuscleMemoryStrength()
{
    const int32 Num = Probabilities.Num();
    if (Num == 0 || Consistencies.Num() != Num)
    {
        UE_LOG(LogTemp, Error, TEXT("[MuscleMemoryFormation] Probabilities and consistencies must be non-empty and of the same length!"));
        return;
    }

    double Sum = InitialMuscleMemory;
    for (int32 i = 0; i < Num; ++i)
    {
        Sum += LearningConstant * Probabilities[i] * Consistencies[i];
    }

    MuscleMemoryStrength = Sum;

    UE_LOG(LogTemp, Log, TEXT("[MuscleMemoryFormation] Muscle Memory Strength (M_m): %f"), (float)MuscleMemoryStrength);
}

double UMuscleMemoryFormationComponent::PredictMuscleMemory(const TArray<double>& Inputs) const
{
    // Placeholder for ML model integration; return current value as stub
    return MuscleMemoryStrength;
}

void UMuscleMemoryFormationComponent::UpdateProbabilitiesAndConsistencies(const TArray<double>& NewProbabilities, const TArray<double>& NewConsistencies)
{
    if (NewProbabilities.Num() != NewConsistencies.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("[MuscleMemoryFormation] New probabilities and consistencies must have the same length!"));
        return;
    }

    Probabilities = NewProbabilities;
    Consistencies = NewConsistencies;
}

