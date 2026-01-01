#include "CognitiveLoadCalculator.h"

DEFINE_LOG_CATEGORY(LogCognitiveLoadCalculator);

UCognitiveLoadCalculator::UCognitiveLoadCalculator()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveLoadCalculator::BeginPlay()
{
    Super::BeginPlay();

    CognitiveLoadValue = CalculateCognitiveLoad();
    UE_LOG(LogCognitiveLoadCalculator, Log, TEXT("Cognitive Load (L_g): %f"), CognitiveLoadValue);
}

float UCognitiveLoadCalculator::InteractionFunction(float InEnvironmental, float InGeological) const
{
    return InEnvironmental * FMath::Loge(1.f + InGeological);
}

float UCognitiveLoadCalculator::CalculateCognitiveLoad() const
{
    const int32 IntegrationSteps = FMath::Max(1, Steps);
    const float DeltaT = (EndTime - StartTime) / static_cast<float>(IntegrationSteps);

    float IntegralSum = 0.f;
    for (int32 Index = 0; Index < IntegrationSteps; ++Index)
    {
        const float CurrentTime = StartTime + Index * DeltaT;
        const float NextTime = CurrentTime + DeltaT;
        const float MidpointTime = 0.5f * (CurrentTime + NextTime);

        const float InteractionValue = InteractionFunction(EnvironmentalStimuli, GeologicalMemory);
        IntegralSum += InteractionValue * DeltaT;
    }

    if (Tau <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCognitiveLoadCalculator, Warning, TEXT("Tau must be greater than zero when calculating cognitive load."));
        return 0.f;
    }

    return IntegralSum / Tau;
}
