#include "FrequencyFluxMeterCalibrationBarWidget.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UFrequencyFluxMeterCalibrationBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Initialize calibration to 100%
    CalibrationFactor = 1.0f;
    if (CalibrationBar)
    {
        CalibrationBar->SetPercent(CalibrationFactor);
    }

    // Bind button clicks
    if (IncrementButton)
    {
        IncrementButton->OnClicked.AddDynamic(this, &UFrequencyFluxMeterCalibrationBarWidget::IncrementCalibration);
    }
    if (DecrementButton)
    {
        DecrementButton->OnClicked.AddDynamic(this, &UFrequencyFluxMeterCalibrationBarWidget::DecrementCalibration);
    }

    // Initialize calibration label
    UpdateCalibrationDisplay();

    // Start repeating timer to simulate and update frequency
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            FrequencyTimerHandle,
            this,
            &UFrequencyFluxMeterCalibrationBarWidget::UpdateFrequency,
            FMath::Max(0.0f, UpdateIntervalSeconds),
            true,
            FMath::Max(0.0f, InitialDelaySeconds));
    }
}

void UFrequencyFluxMeterCalibrationBarWidget::NativeDestruct()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FrequencyTimerHandle);
    }

    if (IncrementButton)
    {
        IncrementButton->OnClicked.RemoveDynamic(this, &UFrequencyFluxMeterCalibrationBarWidget::IncrementCalibration);
    }
    if (DecrementButton)
    {
        DecrementButton->OnClicked.RemoveDynamic(this, &UFrequencyFluxMeterCalibrationBarWidget::DecrementCalibration);
    }

    Super::NativeDestruct();
}

void UFrequencyFluxMeterCalibrationBarWidget::UpdateFrequency()
{
    const float MinVal = FMath::Min(MinFrequency, MaxFrequency);
    const float MaxVal = FMath::Max(MinFrequency, MaxFrequency);
    const float RawFrequency = FMath::FRandRange(MinVal, MaxVal);
    CurrentFrequency = RawFrequency * CalibrationFactor;
    DisplayFrequency(CurrentFrequency);
}

void UFrequencyFluxMeterCalibrationBarWidget::IncrementCalibration()
{
    CalibrationFactor = FMath::Clamp(CalibrationFactor + 0.01f, 0.0f, 1.0f);
    UpdateCalibrationDisplay();
}

void UFrequencyFluxMeterCalibrationBarWidget::DecrementCalibration()
{
    CalibrationFactor = FMath::Clamp(CalibrationFactor - 0.01f, 0.0f, 1.0f);
    UpdateCalibrationDisplay();
}

void UFrequencyFluxMeterCalibrationBarWidget::UpdateCalibrationDisplay()
{
    if (CalibrationBar)
    {
        CalibrationBar->SetPercent(CalibrationFactor);
    }
    if (CalibrationDisplay)
    {
        const float Percent = FMath::Clamp(CalibrationFactor, 0.f, 1.f) * 100.f;
        const FString Text = FString::Printf(TEXT("Calibration: %.0f%%"), Percent);
        CalibrationDisplay->SetText(FText::FromString(Text));
    }
}

void UFrequencyFluxMeterCalibrationBarWidget::DisplayFrequency(float InFrequency)
{
    if (FrequencyDisplay)
    {
        const FString Text = FString::Printf(TEXT("Frequency: %.2f Hz"), InFrequency);
        FrequencyDisplay->SetText(FText::FromString(Text));
    }
}

