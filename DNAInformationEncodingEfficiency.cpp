#include "DNAInformationEncodingEfficiency.h"

DEFINE_LOG_CATEGORY(LogDNAInformationEncodingEfficiency);

UDNAInformationEncodingEfficiency::UDNAInformationEncodingEfficiency()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDNAInformationEncodingEfficiency::BeginPlay()
{
    Super::BeginPlay();

    EncodingEfficiency = CalculateEncodingEfficiency();
    UE_LOG(LogDNAInformationEncodingEfficiency, Log, TEXT("DNA Information Encoding Efficiency (g): %.6f"), EncodingEfficiency);
}

float UDNAInformationEncodingEfficiency::CalculateEncodingEfficiency() const
{
    if (Entropy <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogDNAInformationEncodingEfficiency, Warning, TEXT("Entropy (ΔS) must be greater than zero."));
        return 0.f;
    }

    return GeneticInformationContent / Entropy;
}
