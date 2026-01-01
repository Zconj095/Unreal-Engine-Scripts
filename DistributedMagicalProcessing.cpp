#include "DistributedMagicalProcessing.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogDistributedMagicalProcessing);

UDistributedMagicalProcessing::UDistributedMagicalProcessing()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDistributedMagicalProcessing::BeginPlay()
{
    Super::BeginPlay();
    InitializeNodes(NodeCount);
    const float TotalPower = ComputeTotalPower();
    UE_LOG(LogDistributedMagicalProcessing, Log, TEXT("Total Computational Power: %.4f"), TotalPower);
}

void UDistributedMagicalProcessing::InitializeNodes(int32 InNodeCount)
{
    NodeCount = FMath::Max(1, InNodeCount);
    ProcessingCapabilities.SetNumZeroed(NodeCount);
    EnergyGradients.SetNum(NodeCount);

    PopulateRandomNodes();
}

void UDistributedMagicalProcessing::PopulateRandomNodes()
{
    for (int32 Index = 0; Index < NodeCount; ++Index)
    {
        ProcessingCapabilities[Index] = FMath::FRandRange(0.f, 10.f);
        EnergyGradients[Index] = FVector(
            FMath::FRandRange(-1.f, 1.f),
            FMath::FRandRange(-1.f, 1.f),
            FMath::FRandRange(-1.f, 1.f)
        ).GetSafeNormal();

        if (GetWorld())
        {
            const FVector Origin = GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
            const FVector End = Origin + EnergyGradients[Index] * 100.f;
            DrawDebugLine(GetWorld(), Origin, End, FColor::Purple, true, 5.f, 0, 2.f);
        }
    }
}

float UDistributedMagicalProcessing::ComputeTotalPower() const
{
    float Total = 0.f;
    const int32 Count = FMath::Min(ProcessingCapabilities.Num(), EnergyGradients.Num());
    for (int32 Index = 0; Index < Count; ++Index)
    {
        Total += ProcessingCapabilities[Index] * EnergyGradients[Index].Size();
    }
    return Total;
}

float UDistributedMagicalProcessing::GetProcessingCapability(int32 Index) const
{
    return ProcessingCapabilities.IsValidIndex(Index) ? ProcessingCapabilities[Index] : 0.f;
}

FVector UDistributedMagicalProcessing::GetEnergyGradient(int32 Index) const
{
    return EnergyGradients.IsValidIndex(Index) ? EnergyGradients[Index] : FVector::ZeroVector;
}
