#include "CognitiveLoadBiosynthesis.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoadBiosynthesis);

UCognitiveLoadBiosynthesis::UCognitiveLoadBiosynthesis()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveLoadBiosynthesis::BeginPlay()
{
    Super::BeginPlay();

    CognitiveLoadValue = CalculateCognitiveLoad();
    UE_LOG(LogCognitiveLoadBiosynthesis, Log, TEXT("Cognitive Load for Biosynthesis (L_b): %f"), CognitiveLoadValue);
}

float UCognitiveLoadBiosynthesis::InteractionFunction(float Sm, float Bc) const
{
    return Sm * Bc;
}

float UCognitiveLoadBiosynthesis::CalculateCognitiveLoad() const
{
    const int32 Steps = FMath::Max(1, IntegrationSteps);
    const float Duration = EndTime - StartTime;
    const float Dt = Duration / static_cast<float>(Steps);

    float Sum = 0.f;
    for (int32 Index = 0; Index < Steps; ++Index)
    {
        const float Phi = InteractionFunction(MolecularStimuli, BiochemicalStimuli);
        Sum += Phi * Dt;
    }

    if (ProcessingTimeConstant <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCognitiveLoadBiosynthesis, Warning, TEXT("ProcessingTimeConstant must be greater than zero."));
        return 0.f;
    }

    return Sum / ProcessingTimeConstant;
}
