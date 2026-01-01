#include "FrequencyFluxMeterCalibrationWidget.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UFrequencyFluxMeterCalibrationWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Ensure slider is initialized and bind to value changed
    if (CalibrationSlider)
    {
        // Default Unity behavior uses 1.0 (100%) as starting calibration
        CalibrationSlider->SetValue(1.0f);
        CalibrationSlider->OnValueChanged.AddDynamic(this, &UFrequencyFluxMeterCalibrationWidget::OnCalibrationChanged);
        CalibrationFactor = CalibrationSlider->GetValue();
    }
    else
    {
        CalibrationFactor = 1.0f;
    }

    // Initialize calibration display
    DisplayCalibration(CalibrationFactor);

    // Start repeating timer to simulate and update frequency
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            FrequencyTimerHandle,
            this,
            &UFrequencyFluxMeterCalibrationWidget::UpdateFrequency,
            FMath::Max(0.0f, UpdateIntervalSeconds),
            true,
            FMath::Max(0.0f, InitialDelaySeconds));
    }
}

void UFrequencyFluxMeterCalibrationWidget::NativeDestruct()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FrequencyTimerHandle);
    }

    if (CalibrationSlider)
    {
        CalibrationSlider->OnValueChanged.RemoveDynamic(this, &UFrequencyFluxMeterCalibrationWidget::OnCalibrationChanged);
    }

    Super::NativeDestruct();
}

void UFrequencyFluxMeterCalibrationWidget::UpdateFrequency()
{
    const float MinVal = FMath::Min(MinFrequency, MaxFrequency);
    const float MaxVal = FMath::Max(MinFrequency, MaxFrequency);
    const float RawFrequency = FMath::FRandRange(MinVal, MaxVal);
    CurrentFrequency = RawFrequency * CalibrationFactor;
    DisplayFrequency(CurrentFrequency);
}

void UFrequencyFluxMeterCalibrationWidget::OnCalibrationChanged(float Value)
{
    CalibrationFactor = Value;
    DisplayCalibration(CalibrationFactor);

    // Optionally refresh frequency display immediately
    DisplayFrequency(CurrentFrequency);
}

void UFrequencyFluxMeterCalibrationWidget::DisplayCalibration(float InCalibration01)
{
    if (CalibrationDisplay)
    {
        const float Percent = FMath::Clamp(InCalibration01, 0.f, 1.f) * 100.f;
        const FString Text = FString::Printf(TEXT("Calibration: %.0f%%"), Percent);
        CalibrationDisplay->SetText(FText::FromString(Text));
    }
}

void UFrequencyFluxMeterCalibrationWidget::DisplayFrequency(float InFrequency)
{
    if (FrequencyDisplay)
    {
        const FString Text = FString::Printf(TEXT("Frequency: %.2f Hz"), InFrequency);
        FrequencyDisplay->SetText(FText::FromString(Text));
    }
}

