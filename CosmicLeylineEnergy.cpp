#include "CosmicLeylineEnergy.h"

DEFINE_LOG_CATEGORY(LogCosmicLeylineEnergy);

UCosmicLeylineEnergy::UCosmicLeylineEnergy()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCosmicLeylineEnergy::BeginPlay()
{
    Super::BeginPlay();

    CosmicLeylinePower = CalculateCosmicLeylinePower();
    UE_LOG(LogCosmicLeylineEnergy, Log, TEXT("Cosmic Leyline Power (P_cosmic): %.6f"), CosmicLeylinePower);
}

double UCosmicLeylineEnergy::EnergyDensity(double Time) const
{
    return FMath::Sin(Time) + 2.0;
}

double UCosmicLeylineEnergy::EnergyGradient(double Time) const
{
    return FMath::Cos(Time);
}

double UCosmicLeylineEnergy::CalculateCosmicLeylinePower() const
{
    if (Steps <= 0)
    {
        UE_LOG(LogCosmicLeylineEnergy, Warning, TEXT("Steps must be greater than zero."));
        return 0.0;
    }

    const double Duration = static_cast<double>(TimeEnd) - static_cast<double>(TimeStart);
    if (Duration <= 0.0)
    {
        UE_LOG(LogCosmicLeylineEnergy, Warning, TEXT("TimeEnd must be greater than TimeStart."));
        return 0.0;
    }

    const double DT = Duration / static_cast<double>(Steps);
    double Power = 0.0;

    for (int32 Index = 0; Index < Steps; ++Index)
    {
        const double CurrentTime = static_cast<double>(TimeStart) + Index * DT;
        const double Phi = EnergyDensity(CurrentTime);
        const double Grad = EnergyGradient(CurrentTime);
        Power += Phi * Grad * DT;
    }

    return Power;
}
