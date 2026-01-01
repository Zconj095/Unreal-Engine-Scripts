#include "CognitiveFocusSolarDynamics.h"

DEFINE_LOG_CATEGORY(LogCognitiveFocusSolarDynamics);

UCognitiveFocusSolarDynamics::UCognitiveFocusSolarDynamics()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveFocusSolarDynamics::BeginPlay()
{
    Super::BeginPlay();

    CognitiveCapacity = CalculateCognitiveCapacity();
    UE_LOG(LogCognitiveFocusSolarDynamics, Log, TEXT("Cognitive Capacity (C_s): %.4f"), CognitiveCapacity);
}

float UCognitiveFocusSolarDynamics::CalculateCognitiveCapacity() const
{
    return K * SolarIntensity * PhotonEfficiency;
}
