#include "DiagnosticsVisualizer.h"
#include "Components/TextRenderComponent.h"

DEFINE_LOG_CATEGORY(LogDiagnosticsVisualizer);

UDiagnosticsVisualizer::UDiagnosticsVisualizer()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDiagnosticsVisualizer::BeginPlay()
{
    Super::BeginPlay();

    const double ErrorRate = CalculateAverageErrorRate();
    const FString Message = FString::Printf(TEXT("Average Error Rate: %.4f"), ErrorRate);
    if (DiagnosticsDisplay)
    {
        DiagnosticsDisplay->SetText(FText::FromString(Message));
    }
    UE_LOG(LogDiagnosticsVisualizer, Log, TEXT("%s"), *Message);
}

double UDiagnosticsVisualizer::CalculateAverageErrorRate() const
{
    const int32 SampleCount = FMath::Min(ObservedValues.Num(), TargetValues.Num());
    if (SampleCount == 0)
    {
        return 0.0;
    }

    double Sum = 0.0;
    for (int32 Index = 0; Index < SampleCount; ++Index)
    {
        Sum += FMath::Abs(static_cast<double>(ObservedValues[Index] - TargetValues[Index]));
    }
    return Sum / static_cast<double>(SampleCount);
}
