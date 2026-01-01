#include "DiffractionAnalyzer.h"

DEFINE_LOG_CATEGORY(LogDiffractionAnalyzer);

UDiffractionAnalyzer::UDiffractionAnalyzer()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDiffractionAnalyzer::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogDiffractionAnalyzer, Log, TEXT("Diffraction analyzer ready."));
}

double UDiffractionAnalyzer::ComputeWavelength(int32 Order, double Spacing, double Theta) const
{
    if (Order == 0)
    {
        UE_LOG(LogDiffractionAnalyzer, Warning, TEXT("Order must be greater than zero."));
        return 0.0;
    }

    return (2.0 * Spacing * FMath::Sin(Theta)) / static_cast<double>(Order);
}

double UDiffractionAnalyzer::ComputeAngle(int32 Order, double Wavelength, double Spacing) const
{
    const double Arg = (Order * Wavelength) / (2.0 * Spacing);
    return FMath::Asin(FMath::Clamp(Arg, -1.0, 1.0));
}

double UDiffractionAnalyzer::ComputeSpacing(int32 Order, double Wavelength, double Theta) const
{
    if (FMath::IsNearlyZero(FMath::Sin(Theta)))
    {
        UE_LOG(LogDiffractionAnalyzer, Warning, TEXT("Theta must not produce zero sine."));
        return 0.0;
    }

    return (Order * Wavelength) / (2.0 * FMath::Sin(Theta));
}
