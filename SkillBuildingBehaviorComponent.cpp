#include "SkillBuildingBehaviorComponent.h"

USkillBuildingBehaviorComponent::USkillBuildingBehaviorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USkillBuildingBehaviorComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initial computation (mirrors Unity Start)
    ComputeSkillLevel();
}

void USkillBuildingBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Dynamic update (mirrors Unity Update)
    ComputeSkillLevel();
}

void USkillBuildingBehaviorComponent::ComputeSkillLevel()
{
    // S = S0 * (1 - exp(-kappa * n)) + mu * E_t
    const double RepetitionTerm = MaxSkillLevel * (1.0 - FMath::Exp(-EfficiencyConstant * static_cast<double>(Repetitions)));
    SkillLevel = RepetitionTerm + TransferCoefficient * ErrorCorrection;

    UE_LOG(LogTemp, Log, TEXT("[SkillBuildingBehavior] Skill Level (S): %f"), (float)SkillLevel);
}

double USkillBuildingBehaviorComponent::PredictSkillLevel(const TArray<double>& Inputs) const
{
    // Placeholder for ML integration (mirrors Unity placeholder)
    return SkillLevel;
}

void USkillBuildingBehaviorComponent::UpdateRepetitions(int32 NewRepetitions)
{
    Repetitions = FMath::Max(0, NewRepetitions);
}

void USkillBuildingBehaviorComponent::UpdateErrorCorrection(double NewErrorCorrection)
{
    ErrorCorrection = NewErrorCorrection;
}

