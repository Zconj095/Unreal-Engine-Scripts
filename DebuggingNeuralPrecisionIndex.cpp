#include "DebuggingNeuralPrecisionIndex.h"

DEFINE_LOG_CATEGORY(LogDebuggingNeuralPrecisionIndex);

UDebuggingNeuralPrecisionIndex::UDebuggingNeuralPrecisionIndex()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UDebuggingNeuralPrecisionIndex::BeginPlay()
{
    Super::BeginPlay();
    RecalculateDNPI();
}

void UDebuggingNeuralPrecisionIndex::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    RecalculateDNPI();
    UE_LOG(LogDebuggingNeuralPrecisionIndex, Verbose, TEXT("DNPI: %.6f"), DNPIValue);
}

void UDebuggingNeuralPrecisionIndex::RecalculateDNPI()
{
    if (CognitiveEnergy <= KINDA_SMALL_NUMBER || DebuggingTime <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogDebuggingNeuralPrecisionIndex, Error, TEXT("Cognitive energy (E) and debugging time (T) must be greater than zero."));
        DNPIValue = 0.f;
        return;
    }

    DNPIValue = Gamma * (NDebug / (CognitiveEnergy * DebuggingTime));
}
