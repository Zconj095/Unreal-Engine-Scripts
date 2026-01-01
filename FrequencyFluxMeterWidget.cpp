#include "FrequencyFluxMeterWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UFrequencyFluxMeterWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            FrequencyTimerHandle,
            this,
            &UFrequencyFluxMeterWidget::UpdateFrequency,
            FMath::Max(0.0f, UpdateIntervalSeconds),
            true,
            FMath::Max(0.0f, InitialDelaySeconds));
    }
}

void UFrequencyFluxMeterWidget::NativeDestruct()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FrequencyTimerHandle);
    }

    Super::NativeDestruct();
}

void UFrequencyFluxMeterWidget::UpdateFrequency()
{
    const float MinVal = FMath::Min(MinFrequency, MaxFrequency);
    const float MaxVal = FMath::Max(MinFrequency, MaxFrequency);
    CurrentFrequency = FMath::FRandRange(MinVal, MaxVal);
    DisplayFrequency(CurrentFrequency);
}

void UFrequencyFluxMeterWidget::DisplayFrequency(float InFrequency)
{
    if (FrequencyDisplay)
    {
        const FString Text = FString::Printf(TEXT("Frequency: %.2f Hz"), InFrequency);
        FrequencyDisplay->SetText(FText::FromString(Text));
    }
}

