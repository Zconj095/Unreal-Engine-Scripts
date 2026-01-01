#include "RepetitionLearningComponent.h"

URepetitionLearningComponent::URepetitionLearningComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void URepetitionLearningComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initial computation (mirrors Unity Start)
    ComputeRepetitionLearning();
}

void URepetitionLearningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Dynamic update (mirrors Unity Update)
    ComputeRepetitionLearning();
}

void URepetitionLearningComponent::ComputeRepetitionLearning()
{
    // L_r(t) = L0 * (1 - exp(-gamma * n))
    LearningFromRepetition = MaxLearning * (1.0 - FMath::Exp(-EfficiencyConstant * static_cast<double>(Repetitions)));

    UE_LOG(LogTemp, Log, TEXT("[RepetitionLearning] Learning from Repetition (L_r): %f"), (float)LearningFromRepetition);
}

double URepetitionLearningComponent::PredictLearning(const TArray<double>& Inputs) const
{
    // Placeholder for ML integration (mirrors Unity placeholder)
    return LearningFromRepetition;
}

void URepetitionLearningComponent::SetRepetitions(int32 NewRepetitions)
{
    Repetitions = FMath::Max(0, NewRepetitions);
}

