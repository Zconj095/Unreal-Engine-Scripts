#include "CosmicEnergyAbsorption.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogCosmicEnergyAbsorption);

UCosmicEnergyAbsorption::UCosmicEnergyAbsorption()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCosmicEnergyAbsorption::BeginPlay()
{
    Super::BeginPlay();

    if (Step <= KINDA_SMALL_NUMBER || MaxDistance <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogCosmicEnergyAbsorption, Warning, TEXT("MaxDistance and Step must be positive."));
        return;
    }

    VisualizeEnergyFlux();
}

double UCosmicEnergyAbsorption::CalculateEnergyFlux(double InLuminosity, double Distance)
{
    if (Distance <= 0.0)
    {
        UE_LOG(LogCosmicEnergyAbsorption, Warning, TEXT("Distance must be greater than zero."));
        return 0.0;
    }

    return InLuminosity / (4.0 * PI * Distance * Distance);
}

void UCosmicEnergyAbsorption::VisualizeEnergyFlux() const
{
    if (!GetWorld())
    {
        return;
    }

    const FVector Origin = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
    const double TotalSteps = FMath::Max(1, static_cast<int32>(FMath::CeilToInt(MaxDistance / Step)));

    for (int32 StepIndex = 1; StepIndex <= TotalSteps; ++StepIndex)
    {
        const double Distance = StepIndex * Step;
        const double EnergyFlux = CalculateEnergyFlux(Luminosity, Distance);

        const FVector SphereLocation = Origin + FVector(Distance * 1e-8f, 0.f, 0.f);
        const float Radius = 0.2f;
        const float ColorValue = FMath::Clamp(static_cast<float>(EnergyFlux * 1e-25), 0.f, 1.f);
        const FColor SphereColor = FColor::MakeRedToGreenColorFromScalar(ColorValue);

        DrawDebugSphere(GetWorld(), SphereLocation, Radius, 16, SphereColor, true, 10.f);
        UE_LOG(LogCosmicEnergyAbsorption, Log, TEXT("Distance: %.2e, Energy Flux: %.6e"), Distance, EnergyFlux);
    }
}
