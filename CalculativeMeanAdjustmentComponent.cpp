#include "CalculativeMeanAdjustmentComponent.h"
#include "Logging/LogMacros.h"

UCalculativeMeanAdjustmentComponent::UCalculativeMeanAdjustmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCalculativeMeanAdjustmentComponent::BeginPlay()
{
    Super::BeginPlay();

    if (DataPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("CalculativeMeanAdjustmentComponent: No data points provided."));
        return;
    }

    const float CurrentMean = CalculateMean(DataPoints);
    AdjustDataPointsToMean();

    UE_LOG(LogTemp, Log, TEXT("Current Mean: %f"), CurrentMean);
    UE_LOG(LogTemp, Log, TEXT("Adjusted Mean: %f"), TargetMean);
    FString AdjustedStr;
    for (int32 i = 0; i < DataPoints.Num(); ++i)
    {
        AdjustedStr += FString::SanitizeFloat(DataPoints[i]);
        if (i < DataPoints.Num() - 1)
        {
            AdjustedStr += TEXT(", ");
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Adjusted Data: %s"), *AdjustedStr);
}

float UCalculativeMeanAdjustmentComponent::CalculateMean(const TArray<float>& Values) const
{
    if (Values.Num() == 0)
    {
        return 0.0f;
    }

    double Sum = 0.0;
    for (float V : Values)
    {
        Sum += V;
    }
    return static_cast<float>(Sum / Values.Num());
}

void UCalculativeMeanAdjustmentComponent::AdjustDataPointsToMean()
{
    if (DataPoints.Num() == 0)
    {
        return;
    }

    const float CurrentMean = CalculateMean(DataPoints);
    const float Adjustment = TargetMean - CurrentMean;

    for (float& V : DataPoints)
    {
        V += Adjustment;
    }
}
