#include "CrystalEnergyFlow.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogCrystalEnergyFlow);

UCrystalEnergyFlow::UCrystalEnergyFlow()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCrystalEnergyFlow::BeginPlay()
{
    Super::BeginPlay();

    if (SurfaceNormals.Num() == 0)
    {
        SurfaceNormals = {
            FVector(1.f, 0.f, 0.f),
            FVector(0.f, 1.f, 0.f),
            FVector(0.f, 0.f, 1.f)
        };
    }

    const int32 Count = FMath::Min(SurfaceAreas.Num(), SurfaceNormals.Num());
    if (Count == 0)
    {
        UE_LOG(LogCrystalEnergyFlow, Warning, TEXT("No surface elements defined; aborting flux computation."));
        return;
    }

    double TotalFlux = 0.0;
    for (int32 Index = 0; Index < Count; ++Index)
    {
        const float Area = SurfaceAreas[Index];
        const FVector Normal = SurfaceNormals[Index].GetSafeNormal();
        const double Flux = ComputeFluxElement(Area, Normal);
        TotalFlux += Flux;

        const FVector Origin = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
        const FVector ProbeLocation = Origin + Normal * Area * 10.f;
        DrawDebugLine(GetWorld(), Origin, ProbeLocation, FColor::Cyan, true, 10.f, 0, 2.f);
    }

    UE_LOG(LogCrystalEnergyFlow, Log, TEXT("Total Energy Flux (Φ): %.6f"), TotalFlux);
}

double UCrystalEnergyFlow::ComputeFluxElement(float Area, const FVector& Normal) const
{
    return FVector::DotProduct(MagicalField, Normal) * static_cast<double>(Area);
}

double UCrystalEnergyFlow::ComputeTotalFlux() const
{
    const int32 Count = FMath::Min(SurfaceAreas.Num(), SurfaceNormals.Num());
    double Sum = 0.0;
    for (int32 Index = 0; Index < Count; ++Index)
    {
        Sum += ComputeFluxElement(SurfaceAreas[Index], SurfaceNormals[Index].GetSafeNormal());
    }
    return Sum;
}
